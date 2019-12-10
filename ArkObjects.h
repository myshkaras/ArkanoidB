//
// Created by andre on 13.10.2019.
//

#ifndef ARKANOID_ARKOBJECTS_H
#define ARKANOID_ARKOBJECTS_H

#include <string>
#include <cmath>
#include <vector>
#include <ctime>

struct SPoint
{
    SPoint() = default;
    SPoint(SPoint const & src) = default;
    template<typename T>
    SPoint(T const & x, T const & y) : x(x), y(y) {}
    template<typename T>
    SPoint(T const & t) : x(t.x), y(t.y) {}
    double x{0}, y{0};
    template<typename P>
    SPoint operator = (P const & p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }
    template<typename P>
    SPoint operator -= (P const & p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    template<typename P>
    SPoint operator += (P const & p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }
}; // struct SPoint

inline bool operator == (SPoint const & p1, SPoint const & p2)
{
    return (p1.x==p2.x) && (p1.y==p2.y);
}

inline bool operator != (SPoint const & p1, SPoint const & p2)
{
    return !(p1==p2);
}

inline SPoint operator - (SPoint const & p1, SPoint const & p2)
{
    return {p1.x-p2.x, p1.y-p2.y};
}

inline SPoint operator + (SPoint const & p1, SPoint const & p2)
{
    return {p2.x+p1.x, p2.y+p1.y};
}

inline SPoint operator / (SPoint const & p, double const & d)
{
    return {p.x/d, p.y/d};
}

inline SPoint operator * (SPoint const & p, double const & d)
{
    return {p.x*d, p.y*d};
}

struct SLineWidth
{
    double w{1};
};

struct SColor
{
    double r{0},g{0},b{0};
    bool operator == (SColor const &obj) {
        if((r == obj.r)&&(g == obj.g)&&(b == obj.b)) return true; else return false;
    }
};

struct SLine
{
    SPoint a{0,0};
    SPoint b{0,0};
};

struct SRect
{
    SRect()=default;
    SRect(const SPoint& _p, const SPoint& _s);

    virtual ~SRect();

    SPoint p{0,0};
    SPoint s{0,0};
};

struct Brick: public SRect
{
public:

    Brick()= default;
    Brick(const SPoint& _p, const SPoint& _s, const u_int8_t _hitCount);

    Brick (const Brick &brick);

    virtual ~Brick();

    SColor const & GetColorByHit() const;

    int numberOfColors{9};
    u_int8_t  hitCount{0};
};



#endif //ARKANOID_ARKOBJECTS_H
