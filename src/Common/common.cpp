#include "common.h"

namespace Xcurse
{
    bool Position::operator<(const Position &that) const
    {
        return (y == that.y) ? (x < that.x) : (y < that.y);
    }

    bool Position::operator==(const Position &that) const
    {
        return (x == that.x) && (y == that.y);
    }

    Position &Position::operator+=(const Position &that)
    {
        x += that.x;
        y += that.y;
        return *this;
    }

    // Position operator+(Position p1, const Position &p2)
    // {
    //     p1 += p2;
    //     return p1;
    // }

    bool Size::operator==(Size &that)
    {
        return width == that.width && height == that.height;
    }
}