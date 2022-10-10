#pragma once

#include "SingletonShape.h"
#include <cmath>

namespace Xcurse
{
    class Circle : public SingletonShape
    {
    public:
        Circle();
        Circle(Position midpoint, int radius, Size restraint = {0, 0});

        std::vector<Position> &rasterise() override;

        void set_radius(int r);
        void set_midpoint(Position p);
        void set_restraint(Size res);

    private:
        Position midpoint;
        int radius;
    };
}