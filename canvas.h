//
// Created by root on 08.10.2019.
//

#ifndef ARKANOID_CANVAS_H
#define ARKANOID_CANVAS_H

#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include "drawingPrimitives.h"
#include <vector>
#include "ArkObjects.h"

enum LevelState{
    lsInit = 0,
    lsPlay = 1,
    lsEnd = 2};

struct SCollision
{
    double d       {  1e9  };
    double dAngle  {  0.0  };
    SPoint tWhere  { .0,.0 };
    SPoint tOffset { .0,.0 };
    enum class EWhat
    {
        none,
        Element
    } eWhat { EWhat::none };
    size_t nIndex { 0 };
    size_t nSubIx { 0 };
};

using VButtons = std::vector<SButton>;
using VBricks = std::vector<Brick>;

struct steps
{
    double s = 2;
    SPoint   m_tCtxSize     { .0,.0 };
    SPoint pgpos{SPoint{-m_tCtxSize.x, -m_tCtxSize.y}/2+SPoint{50,100}};

    double xStep{12};
    double yStep{16};

    double w = (m_tCtxSize.x-100)/(xStep);
    double h = (m_tCtxSize.y-100)/(yStep);

    SPoint pgsiz{w-2*s, h-2*s};
};


class Arkanoid : public Gtk::DrawingArea
{
public:
    Arkanoid();

    virtual ~Arkanoid();

    void setParentWindow(Gtk::Window * pWindow);
    void closeWindow();

    void setNewLevel(const std::string& S);

    void Init();

    void updateSteps ();

    steps st;

    VBricks m_vBricks;
    SRect m_tPlayground{{0,0},{1,1}};
    Brick   m_tPaddle;
    SColor BallColor;
    SPoint  m_tBall;
    bool    m_bBallFree{false};
    bool    m_bBricksInitialized{false};

    bool m_bButtonDown{false};

private:
    bool runOnce{false};

protected:
    Gtk::Window * parentWindow{nullptr};

    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_scroll_event(GdkEventScroll *event) override;
    bool on_button_press_event(GdkEventButton * event) override;
    bool on_motion_notify_event(GdkEventMotion *event) override;
    bool on_button_release_event(GdkEventButton* release_event) override;

    bool Collision(SPoint const & tPoint);

    double   m_dScale       { 1.0 };

 //   bool     m_bShiftInit   { true };

    SPoint   m_tShift       { .0,.0 };
    SPoint   m_tEventPress  { .0,.0 };
    SPoint   m_tEventRelease{ .0,.0 };
    SPoint   m_tShiftStart  { .0,.0 };

    SPoint   m_tMousePos;

    SCollision m_tCollision;

    // animation
    bool             m_bAnimate{true};
    bool             m_bAnimateLeft{true};

    // animation clock
    double           m_dAnimator{0}; // $m_tAnimator animation parameter
    double           m_dAnimStep{0}; // intermediate animation parameter
    sigc::slot<bool> m_fSlot;
    sigc::connection m_afConnections;

    double           m_dAnimate   {0.0025}; // animation steps width
    double const     m_dAnimateMax{0.0250}; // maximal animation step width
    double const     m_dAnimateMin{0.0025}; // minimal animation step width
/*
        double m_nTick{.0};
        bool Animate(int c) // vibration
            {
            if (!m_bAnimate) return true;
            m_dAnimator = (m_dAnimator > 0) ? 0 : 1;
            queue_draw();
            return true;
            }
*/
    bool Animate(int c) // rotation
    {
        if (!m_bAnimate) return true;
        if (m_bAnimateLeft)
            m_dAnimator = (m_dAnimator <=  m_dAnimate) ? 1 : m_dAnimator-m_dAnimate;
        else
            m_dAnimator = (m_dAnimator >=1-m_dAnimate) ? 0 : m_dAnimator+m_dAnimate;
        queue_draw();
        return true;
    }
/*
        bool Animate(int c) // bidirectional
            {
            if (!m_bAnimate) return true;
            if (m_bAnimateLeft)
                m_dAnimator -= m_dAnimate;
                else
                m_dAnimator += m_dAnimate;
            if (m_dAnimator <= 0) m_bAnimateLeft = false;
            if (m_dAnimator >= 1) m_bAnimateLeft = true;
            queue_draw();
            return true;
            }
*/

    VButtons    m_voButtons;            // button bar
    std::string m_oButtonPressed{""};   // last pressed button

    LevelState CurrentState{lsInit};
    time_t GameStarted{0};
    time_t GameEnded{0};
    uint64_t Score{0};
    std::string CurrLevelName{""};

}; // Arkanoid


#endif //ARCANOID_CANVAS_H
