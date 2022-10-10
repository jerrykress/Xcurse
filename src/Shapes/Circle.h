#pragma once

#include "GenericShapeObject.h"
#include <cmath>
#include <ctgmath>

#define PI 3.14159265

namespace Xcurse
{
    class Circle : public GenericShapeObject
    {
    public:
        Circle();
        Circle(Position midpoint, int radius, Size restraint = {0, 0});

        std::vector<Position> &rasterise() override;

        void set_radius(int r);
        void set_midpoint(Position p);
        void set_restraint(Size res);

        Position get_at_angle(float a);
        Position get_at_degree(float d);

    private:
        Position midpoint;
        int radius;
    };
}