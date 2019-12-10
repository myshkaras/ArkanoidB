//
// Created by andre on 13.10.2019.
//

#ifndef ARKANOID_DRAWINGPRIMITIVES_H
#define ARKANOID_DRAWINGPRIMITIVES_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include "ArkObjects.h"

struct SButton
{
    double x{0},y{0},w{0},h{0};
    std::string sTitle;
    std::string sFunct;

    SButton (double const & x, double const & y,
             double const & w, double const & h,
             std::string const t,
             std::string const f)
            : x(x), y(y), w(w), h(h), sTitle(t), sFunct(f) {}

    bool Collision(SPoint const & p) const
    {
        return ( p.x > x && p.x < x+w && p.y > y && p.y < y+h );
    }
};

SColor constexpr BLACK      {.0,.0,.0};
SColor constexpr DARKRED    {.6,.0,.0};
SColor constexpr DARKGREEN  {.0,.6,.0};
SColor constexpr DARKBLUE   {.0,.0,.6};
SColor constexpr DARKCYAN   {.0,.6,.6};
SColor constexpr DARKMAGENTA{.6,.0,.6};
SColor constexpr DARKYELLOW {.6,.6,.0};
SColor constexpr GRAY       {.6,.6,.6};
SColor constexpr RED        {1,0,0};
SColor constexpr GREEN      {0,1,0};
SColor constexpr BLUE       {0,0,1};
SColor constexpr CYAN       {0,1,1};
SColor constexpr MAGENTA    {1,0,1};
SColor constexpr YELLOW     {1,1,0};
SColor constexpr WHITE      {1,1,1};

int constexpr TEXT_LEFT   {0};
int constexpr TEXT_MIDDLE {1};
int constexpr TEXT_RIGHT  {2};

using CairoCtx = Cairo::RefPtr<Cairo::Context> const &;
// drawing primitives
inline void LineWidth( CairoCtx cr, SLineWidth const & w, double const & s=1 )
{
    cr->set_line_width(w.w / ((s==0)?1:s));
}

inline void Color( CairoCtx cr, SColor const & c )
{
    cr->set_source_rgb( c.r, c.g, c.b );
}

inline void Color( CairoCtx cr, SColor const & c, double const & a )
{
    cr->set_source_rgba( c.r, c.g, c.b, a );
}

inline void Color( CairoCtx cr, double const & r, double const & g, double const & b )
{
    cr->set_source_rgb( r, g, b );
}

inline void Color( CairoCtx cr, double const & r, double const & g, double const & b, double const & a )
{
    cr->set_source_rgba( r, g, b, a );
}

///////////////////////////////////////////    DISTANCE
// the first argument can be anything having the members x and y
// a Gtk event, a SFleck or whatever
template<typename P, typename T>
inline double Distance( P const & a, T const & b )
{
    return sqrt( pow((a.x-b.x),2) + pow((a.y-b.y),2) );
}


template<typename P>
void MoveTo(CairoCtx cr, P const & tPoint)
{
    cr->move_to(tPoint.x, tPoint.y);
}

template<typename P>
void LineTo(CairoCtx cr, P const & tPoint)
{
    cr->line_to(tPoint.x, tPoint.y);
}

template<typename S, typename... P>
void Line(CairoCtx cr, S const & tPoint1, P const &... tPoints )
{
    MoveTo(cr, tPoint1);
    (LineTo(cr, tPoints), ...);
    cr->stroke();
}

inline void Line(CairoCtx cr, SLine const & tLine )
{
    MoveTo(cr, tLine.a);
    LineTo(cr, tLine.b);
    cr->stroke();
}

template<typename C>
void LineStrip(CairoCtx cr, C const & tPoints )
{
    bool b{true};
    for ( auto const & a:tPoints )
    {
        if (b)
        {
            MoveTo(cr, a);
            b = false;
        }
        else
        {
            LineTo(cr, a);
        }
    }
    cr->stroke();
}

template<typename S, typename... P>
void Polygon(CairoCtx cr, S const & tPoint1, P const &... tPoints )
{
    MoveTo(cr, tPoint1);
    (LineTo(cr, tPoints), ...);
    LineTo(cr, tPoint1);
    cr->fill();
}

template<typename C>
void Polygon(CairoCtx cr, C const & tPoints )
{
    bool b{true};
    for ( auto const & a:tPoints )
    {
        if (b) MoveTo(cr, a); else LineTo(cr, a);
    }
    cr->fill();
}

template<typename P>
void Circle(CairoCtx cr, P const & tPoint, double const & dRadius )
{
    cr->arc(tPoint.x, tPoint.y, dRadius, 0, 2*M_PI);
    cr->fill();
}

template<typename P>
void Ring(CairoCtx cr, P const & tPoint, double const & dRadius )
{
    cr->arc(tPoint.x, tPoint.y, dRadius, 0, 2*M_PI);
    cr->stroke();
}

template<typename R>
void Rectangle(CairoCtx cr, R const & tRect )
{
    cr->rectangle(tRect.p.x, tRect.p.y, tRect.s.x, tRect.s.y);
    cr->fill();
}

template<typename P, typename S>
void Rectangle(CairoCtx cr, P const & tPoint, S const & tSize )
{
    cr->rectangle(tPoint.x, tPoint.y, tSize.x, tSize.y);
    cr->fill();
}

template<typename R>
void Frame(CairoCtx cr, R const & tRect )
{
    cr->rectangle(tRect.p.x, tRect.p.y, tRect.s.x, tRect.s.y);
    cr->stroke();
}

template<typename P, typename S>
void Frame(CairoCtx cr, P const & tPoint, S const & tSize )
{
    cr->rectangle(tPoint.x, tPoint.y, tSize.x, tSize.y);
    cr->stroke();
}


inline SPoint
Text(const Cairo::RefPtr<Cairo::Context> &cr, SPoint const &pos, std::string const &crsText, const double &dScale = 1.0,
     const bool &label = false, const int &nOrintation = TEXT_MIDDLE, const double fontSize = 12.0)
{
    cr->save();

    Pango::FontDescription font;

    font.set_family("Sans");
    font.set_absolute_size(fontSize * PANGO_SCALE*dScale);

    Gtk::DrawingArea w;
    auto layout = w.create_pango_layout(crsText);

    layout->set_font_description(font);
    int iWidth {0};
    int iHeight{0};
    SPoint tSize{.0,.0};

    layout->get_pixel_size(iWidth, iHeight);
    tSize = SPoint{iWidth, iHeight};

    SPoint tPos;
    switch (nOrintation)
    {
        case TEXT_LEFT:   tPos = {pos.x          , pos.y}; break;
        case TEXT_MIDDLE: tPos = {pos.x-tSize.x/2, pos.y}; break;
        case TEXT_RIGHT:  tPos = {pos.x-tSize.x  , pos.y}; break;
    }

    if (label)
    {
        cr->save();
        Color(cr, GRAY, .75);
        LineWidth(cr, {.0+iHeight});
        Line(cr, SLine{{tPos.x        +tSize.y/4, tPos.y},
                       {tPos.x+tSize.x-tSize.y/4, tPos.y}});
        cr->restore();
    }

    switch (nOrintation)
    {
        case TEXT_LEFT:   cr->move_to(pos.x          , pos.y-tSize.y/2); break;
        case TEXT_MIDDLE: cr->move_to(pos.x-tSize.x/2, pos.y-tSize.y/2); break;
        case TEXT_RIGHT:  cr->move_to(pos.x-tSize.x  , pos.y-tSize.y/2); break;
    }

    layout->show_in_cairo_context(cr);
    cr->restore();

    return std::move(tSize);
}


#endif //ARKANOID_DRAWINGPRIMITIVES_H
