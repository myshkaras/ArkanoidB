//
// Created by andre on 19.10.2019.
//

#include "ArkObjects.h"
#include "drawingPrimitives.h"

SRect::SRect(const SPoint& _p, const SPoint& _s) :
        p(_p),
        s(_s)
{
}

SRect::~SRect() {
}

Brick::Brick(const SPoint& _p, const SPoint& _s, const u_int8_t _hitCount) :
                                        SRect(_p, _s),
                                        hitCount(_hitCount)
{
}

Brick::~Brick()
{
}

/*
SColor const & Brick::GetColorByHit()const {
    switch (hitCount) {
        case 0 :
            return BLACK;
            break;
        case 1 :
            return DARKRED;
            break;
        case 2 :
            return DARKGREEN;
            break;
        case 3 :
            return DARKBLUE;
            break;
        case 4 :
            return DARKCYAN;
            break;
        case 5 :
            return DARKMAGENTA;
            break;
        case 6 :
            return DARKYELLOW;
            break;
        case 7 :
            return GRAY;
            break;
        case 8 :
            return RED;
            break;
        case 9 :
            return GREEN;
            break;
        case 10 :
            return BLUE;
            break;
        case 11 :
            return CYAN;
            break;
        case 12 :
            return MAGENTA;
            break;
        case 13 :
            return YELLOW;
            break;
        case 14 :
            return WHITE;
            break;
        default:
            return GREEN;
            break;
    }
}
*/

SColor const & Brick::GetColorByHit()const {
    switch (hitCount) {
        case 0 :
            return GRAY;
            break;
        case 1 :
            return RED;
            break;
        case 2 :
            return GREEN;
            break;
        case 3 :
            return BLUE;
            break;
        case 4 :
            return CYAN;
            break;
        case 5 :
            return MAGENTA;
            break;
        case 6 :
            return YELLOW;
            break;
        case 7 :
            return WHITE;
            break;
        case 8 :
            return DARKBLUE;
            break;
        default:
            return GREEN;
            break;
    }
}

Brick::Brick(const Brick &brick)
{
    this->p = brick.p;
    this->s = brick.s;
    this->hitCount = brick.hitCount;
    this->numberOfColors = brick.numberOfColors;
}

