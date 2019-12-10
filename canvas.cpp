//
// Created by root on 08.10.2019.
//

#include "canvas.h"
#include "drawingPrimitives.h"
#include "ArkObjects.h"

#include "pugixml.hpp"


Arkanoid::Arkanoid()
{
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::SCROLL_MASK | Gdk::SMOOTH_SCROLL_MASK);
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK);
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

    m_fSlot         = sigc::bind(sigc::mem_fun(*this, &Arkanoid::Animate), 0);
    m_afConnections = Glib::signal_timeout().connect(m_fSlot, 20);

    for ( int i{0}; i<13; ++i)
    {
        auto constexpr bs {38.0};  // button size
        auto constexpr uix{20.0},uiy{20.0},uiw{bs},uih{bs};
        auto constexpr bsp{ 8.0};  // button spacing
        auto constexpr bbo{16.0};  // button bar offset
        m_voButtons.emplace_back( bbo+i*(uiw+bsp), bbo+uiy, uiw, uih,
                                  std::to_string(i),
                                  std::to_string(i) );
    }
}

Arkanoid::~Arkanoid()
{
    m_afConnections.disconnect();
};

 bool Arkanoid::Collision(SPoint const & p)
    {
    m_tCollision.eWhat = SCollision::EWhat::none;
    size_t i{0};
    for ( auto const & a:m_vBricks )
        {
//      if ( p.x > a.p.x && p.x < a.p.x+a.st.x && p.y > a.p.y && p.y < a.p.y+a.st.y )
        if ( p.x+11 > a.p.x && p.x-11 < a.p.x+a.s.x && p.y+11 > a.p.y && p.y-11 < a.p.y+a.s.y )
            {
            m_tCollision.eWhat = SCollision::EWhat::Element;
            m_tCollision.nIndex = i;
            return true;
            }
        ++i;
        }

    return false;
    }

bool Arkanoid::on_button_press_event(GdkEventButton *event)
{
    m_tMousePos = (*event - m_tShift)/m_dScale;

    for ( auto const & a:m_voButtons )
    {
        if ( a.Collision(*event) )
        {
            m_oButtonPressed = a.sFunct;
            return true;
        }
    }

    if (event->type == GDK_BUTTON_PRESS )
    {
        m_tEventPress = *event;
        m_tShiftStart = m_tShift;
    }
    else
    {
//      auto const bCol { Collision(m_tMousePos) };
    }

    m_bButtonDown = true;

    queue_draw();
    return true;
}

bool Arkanoid::on_motion_notify_event(GdkEventMotion *event)
{
    m_tMousePos = (*event - m_tShift)/m_dScale;

    if ( m_oButtonPressed > "") return true;

    if ( event->type & GDK_MOTION_NOTIFY )
    {
        if ( event->state & GDK_BUTTON3_MASK )
        {
//          m_tShift = m_tShiftStart - (m_tEventPress - *event);
        }
        else if ( event->state & GDK_BUTTON1_MASK )
        {
//          Collision(m_tMousePos);
        }
        else
        {
//          auto const bCol { Collision(m_tMousePos) };
        }
    }

    queue_draw();
    return true;
}

bool Arkanoid::on_button_release_event(GdkEventButton* event)
{
    m_tMousePos = (*event - m_tShift)/m_dScale;
    m_bButtonDown = false;

    if ( m_oButtonPressed.size() > 0 )
    {
        if ( event->state & GDK_BUTTON1_MASK )
        {
            m_oButtonPressed = "";
            for ( auto const & a:m_voButtons )
                if ( a.Collision(*event) )
                {
                    m_oButtonPressed = a.sFunct;
                    break;
                }

            if ( m_oButtonPressed == "0" )
            {
                m_vBricks.clear();
                CurrentState = lsEnd;
                GameEnded = time(nullptr);

            } else
/*            if ( m_oButtonPressed == "1" )
            {
                setNewLevel(m_oButtonPressed);
            }
            if ( m_oButtonPressed == "2" ) {}
            if ( m_oButtonPressed == "3" ) {}
            if ( m_oButtonPressed == "4" ) {}
*/

            if ( m_oButtonPressed == "X" )
            {
                parentWindow->close();
            } else{
                setNewLevel(m_oButtonPressed);
            }
        }
        m_oButtonPressed="";
        queue_draw();
        return true;
    }

    if ( event->type & GDK_MOTION_NOTIFY )
        if ( event->state & GDK_BUTTON1_MASK )
        {
//          Collision(m_tMousePos);
        }
    if ( event->state & GDK_BUTTON3_MASK )
    {
        m_bBallFree = true;
        BallColor = m_tPaddle.GetColorByHit();
        if(CurrentState == lsInit)
        {
            CurrentState = lsPlay;
            GameStarted = time(nullptr);
        }
    }

    queue_draw();
    return true;
}

bool Arkanoid::on_scroll_event(GdkEventScroll *event)
{
    //return true;

    /*SPoint const p0{ (*event - m_tShift)/m_dScale };
    m_dScale *= (event->delta_y>0)?.9:1.1; if (m_dScale<.01) m_dScale=.01;
    SPoint const p1{ (*event - m_tShift)/m_dScale };
    m_tShift -= (p0-p1)*m_dScale;*/
    m_tPaddle.hitCount = (m_tPaddle.hitCount + 1) % m_tPaddle.numberOfColors;
    if(!m_bBallFree)
    {
        BallColor = m_tPaddle.GetColorByHit();
    }
    queue_draw();
    return true;
}

bool Arkanoid::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
    updateSteps();

    static auto tHome{SPoint {st.m_tCtxSize } / 2 };

    if(!runOnce)
    {
        Init();
        tHome = m_tShift = st.m_tCtxSize / 2;
        runOnce = true;
    }

/*    if ( m_bShiftInit )
    {
        tHome = m_tShift = st.m_tCtxSize / 2;
        m_bShiftInit = false;
    }*/

    auto const tSizeHalf{st.m_tCtxSize / 2};
    if ( tHome != tSizeHalf )
    {
        m_tShift -= tHome - tSizeHalf; tHome = tSizeHalf;
    }

    Cairo::Matrix matrix(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    matrix.scale(m_dScale,m_dScale);
    matrix.translate(m_tShift.x/m_dScale, m_tShift.y/m_dScale);

    cr->transform(matrix);

    ////////////////////  PLAYGROUND  ////////////////////////

    cr->set_line_width(1/m_dScale);
    Color( cr, BLACK );
    Rectangle(cr, SPoint{-st.m_tCtxSize.x, -st.m_tCtxSize.y} / 2, SPoint{st.m_tCtxSize.x, st.m_tCtxSize.y} );
    Color( cr, GRAY );
    Rectangle( cr, m_tPlayground );

    /////////////////////  PADDLE  /////////////////////

    if ( m_tMousePos.x < m_tPlayground.p.x+m_tPaddle.s.x/2 )
        m_tPaddle.p.x = m_tPlayground.p.x;
    else if ( m_tMousePos.x > m_tPlayground.p.x+m_tPlayground.s.x-m_tPaddle.s.x/2 )
        m_tPaddle.p.x = m_tPlayground.p.x+m_tPlayground.s.x-m_tPaddle.s.x;
    else
        m_tPaddle.p.x = m_tMousePos.x - m_tPaddle.s.x/2;
    Color( cr, m_tPaddle.GetColorByHit() );
    Rectangle( cr, m_tPaddle );
    Color( cr, BLACK );
    Frame( cr, m_tPaddle );

    //////////////////////  BALL  /////////////////////

    if ( !m_bBallFree )
    {
        if ( m_tMousePos.x < m_tPlayground.p.x+11 )
            m_tBall.x = m_tPlayground.p.x+11;
        else if ( m_tMousePos.x > m_tPlayground.p.x+m_tPlayground.s.x-11 )
            m_tBall.x = m_tPlayground.p.x+m_tPlayground.s.x-11;
        else
            m_tBall.x = m_tMousePos.x;
    }
    else
    {
        ///Ball is captured
    }

    Color( cr, BallColor );
    Circle( cr, m_tBall, 11 );
    Color( cr, BLACK );
    Ring( cr, m_tBall, 11 );

    static double m_nTick{m_dAnimator};
    if ( m_bBallFree && (m_nTick != m_dAnimator) )
    {
        m_nTick = m_dAnimator;
        static double m{0};
        static double s{.018};
        static double u{1.2};
        if (Collision(m_tBall)) {
            s = -s;
        }
        m += s;
        if (m > 1.0 - abs(s) || m < abs(s))
        {
            s = -s;
            if ( s > 0 )
            {
                BallColor = m_tPaddle.GetColorByHit();
                if ( m_tBall.x < (m_tPaddle.p.x - 11) || m_tBall.x > (m_tPaddle.p.x + m_tPaddle.s.x + 11) )
                {
                    Score = 0;
                    m_bBallFree = false;
                    u = 1.2;
                }
                else
                {
                    u = (m_tBall.x - (m_tPaddle.p.x+m_tPaddle.s.x/2)) / ( m_tPaddle.s.x/5 );
                }
            }
        }
        m_tBall.y = st.m_tCtxSize.y/2-m_tPaddle.s.y-11 - m*(m_tPlayground.s.y-m_tPaddle.s.y-11*2);

        m_tBall.x += u;
        if ( m_tBall.x < m_tPlayground.p.x+11 || m_tBall.x > m_tPlayground.p.x+m_tPlayground.s.x-11 )
        {
            u = -u;
        }
    }

    //////////////////////  BRICKS  /////////////////////////

    if ( m_tCollision.eWhat == SCollision::EWhat::Element )
    {
        //BRICK was hit
        SColor c =m_vBricks[m_tCollision.nIndex].GetColorByHit();

//      Calculate score of rhe HIT
        Score+=m_tPaddle.hitCount*13;

//      if((BallColor.b == c.b)&&(BallColor.g == c.g)&&(BallColor.r == c.r) ) {
        if(BallColor == c) {
            m_vBricks.erase(m_vBricks.begin() + m_tCollision.nIndex);
            if((CurrentState == lsPlay) &&(m_vBricks.empty())){
                CurrentState = lsEnd;
                GameEnded = time(nullptr);
            }
        }else{
           m_vBricks[m_tCollision.nIndex].hitCount = (m_vBricks[m_tCollision.nIndex].hitCount + m_tPaddle.hitCount) % m_tPaddle.numberOfColors;
        }
    }

    int j=0;
    // LineWidth(cr, {m_tCtxSize.y/15/m_dScale});
    LineWidth(cr, {1/m_dScale});
 //   Color(cr, GREEN);
    for ( auto const & a:m_vBricks )
    {
        //Color(cr, GREEN);
        Color(cr, a.GetColorByHit());
        Rectangle(cr, a);
        Color(cr, BLACK);
        Frame(cr, a);
        ++j;
    }

    /////////////////////  BUTTONS  //////////////////////////
    int i=0;
    SPoint const tMousePos{m_tMousePos*m_dScale+m_tShift};
    for ( auto const & a:m_voButtons )
    {
        ++i;
        cr->set_line_width(1/m_dScale);

        if ( a.Collision(tMousePos) )
        {
            Color( cr, GREEN, .8 );
        }
        else
        {
            if(a.sTitle == CurrLevelName)
            {
                Color( cr, MAGENTA );
            } else{
                Color( cr, {.7,.7,.7}, .8 );
            }
        }

        Rectangle( cr, (a-m_tShift)/m_dScale, SPoint{a.w/m_dScale, a.h/m_dScale} );
        cr->set_source_rgb(.2,.2,.2);
        Frame    ( cr, (a-m_tShift)/m_dScale, SPoint{a.w/m_dScale, a.h/m_dScale} );

        Color( cr, BLACK );
        Text(cr, {(a.x + a.w / 2 - m_tShift.x) / m_dScale,
                  (a.y + a.h / 2 - m_tShift.y) / m_dScale}, a.sTitle,1 / m_dScale,0);
    }

    /////////////////////////  SCORE  //////////////////////
    std::string sScore{"SCORE:"};
    sScore.append(std::to_string(Score));

    BallColor == BLACK ? Color(cr, WHITE) : Color(cr, BallColor);

    Text(cr, (SPoint{-500.0, 50.0} + SPoint{st.m_tCtxSize.x, .0} - m_tShift) / m_dScale, sScore, 1 / m_dScale, false,
            TEXT_RIGHT, 24.0);

    /////////////////////////  CLOCK  //////////////////////
    auto t = std::time(nullptr);
    char sDate[100];//={'G','a','m','e', ':',' ',' '};
    time_t delta;

    switch(CurrentState) {
        case lsInit:
            std::strftime(sDate, sizeof(sDate), "Time: %H:%M:%S", std::localtime(&t));
            Color(cr, WHITE);
            break;

        case lsPlay:
            delta = t - GameStarted;
            std::strftime(sDate, sizeof(sDate), "Game: %H:%M:%S", std::gmtime(&delta));
            Color(cr, YELLOW);
            break;

        case lsEnd:
            delta = GameEnded - GameStarted;
            std::strftime(sDate, sizeof(sDate), "Played for: %H:%M:%S", std::gmtime(&delta));
            Color(cr, CYAN);
            break;

        default: ;
    }
    Text(cr, (SPoint{-150.0, 50.0} + SPoint{st.m_tCtxSize.x, .0} - m_tShift) / m_dScale, sDate, 1 / m_dScale, false,
         TEXT_RIGHT, 24.0);

    m_tCollision.eWhat = SCollision::EWhat::none;

    return true;
}

void Arkanoid::Init() {
     //Add EXIT button
    auto constexpr bs {38.0};  // button size
    auto constexpr uix{20.0},uiy{20.0},uiw{bs},uih{bs};
    auto constexpr bsp{ 8.0};  // button spacing
    auto constexpr bbo{16.0};  // button bar offset

    m_voButtons.emplace_back( parentWindow->get_allocation().get_width() - bs - bbo, bbo, uiw, uih,
                              "X",
                              "X" );

    //Bricks initialisation
    setNewLevel(m_voButtons[0].sTitle);

    m_tPlayground = {st.pgpos, SPoint{st.m_tCtxSize.x, st.m_tCtxSize.y} - SPoint{100, 100} };

    CurrentState = lsInit;

    m_tPaddle = Brick{SPoint{.0, st.m_tCtxSize.y / 2 - st.pgsiz.y}, st.pgsiz, 5 };
    BallColor = m_tPaddle.GetColorByHit();
    m_tBall   = SPoint{.0, st.m_tCtxSize.y / 2 - st.pgsiz.y - 11};
}

void Arkanoid::setParentWindow(Gtk::Window *pWindow)
{
     parentWindow = pWindow;
}

void Arkanoid::closeWindow()
{
    if(parentWindow)
    {
        parentWindow->close();
    }
}

void Arkanoid::setNewLevel(const std::string &S)
{
     if(!m_vBricks.empty())
     {
         m_vBricks.clear();
     }

     updateSteps();

     Score = 0;

     pugi::xml_document doc;

     std::string xml{g_get_current_dir ()};
     xml.append("/config.xml");

//  if (!doc.load_file("/home/andre/Desktop/coursework/Arkanoid/xgconsole.xml"))
    if (!doc.load_file(xml.c_str()))
    {
//        m_voButtons[0].sTitle = "Err";
        return;
    }
//    m_voButtons[0].sTitle = "Ok";

    CurrentState = lsInit;

    std::string cLevel{"Level"};
    cLevel.append(S);
    pugi::xml_node Level = doc.child("Levels").child(cLevel.c_str());

    //Check Level exist
    if(Level.name() != cLevel)
    {
        CurrLevelName = m_voButtons[0].sTitle;
    } else{
        CurrLevelName = S;
    }

    int id(0);

    for (int x{0}; x < st.xStep; ++x)
    {
        for (int y{0}; y < st.yStep / 2; ++y)
        {
            pugi::xml_node_iterator foundBrick = Level.end();
            for (pugi::xml_node_iterator brick = Level.begin(); brick != Level.end(); ++brick)
            {
                if (brick->attribute("ID").as_int()==id)
                {
                    foundBrick = brick;
                }
            }

            if (foundBrick==Level.end())
            {
                m_vBricks.emplace_back(Brick{st.pgpos + SPoint{st.w * x + st.s, st.h * y + st.s}, st.pgsiz, u_int8_t(y)});
            }
            else if(foundBrick->attribute("HP").as_int()!=0)
            {
                m_vBricks.emplace_back(Brick{st.pgpos + SPoint{st.w * x + st.s, st.h * y + st.s}, st.pgsiz, uint8_t(foundBrick->attribute("HP").as_int())});
            }
            id++;
        }
    }
    ///////////////////// Exclude brick from level, Two methods. Obsolete////////////////
/*
    VBricks::iterator it = m_vBricks.begin();
    while (it != m_vBricks.end()) {
        if (it->hitCount==0)
            it = m_vBricks.erase(it); // after erasing, 'it' will be set to the next element in v
        else
            ++it;             // manually set 'it' to the next element in v
    }
*/

/*
    for ( auto const & a:m_vBricks )
    {
        m_vBricks.erase(std::remove_if(m_vBricks.begin(), m_vBricks.end(), [](auto& element){return element.hitCount==0;}), m_vBricks.end()); //Use lambda function to provide predicate
    }
*/

}

void Arkanoid::updateSteps()
{
    auto const all        { get_allocation() };
    st.m_tCtxSize = {SPoint {(double)all.get_width(),
                             (double)all.get_height() } };

    st.pgpos = {SPoint{-st.m_tCtxSize.x, -st.m_tCtxSize.y} / 2 + SPoint{50, 100}};

    st.xStep =12;
    st.yStep = 16;

    st.s = 2;

    st.w = (st.m_tCtxSize.x - 100) / (st.xStep);
    st.h = (st.m_tCtxSize.y - 100) / (st.yStep);

    st.pgsiz = {st.w - 2 * st.s, st.h - 2 * st.s};
}
