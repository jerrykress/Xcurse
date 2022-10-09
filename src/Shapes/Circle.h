#pragma once

#include "SingletonShape.h"
#include <cmath>

namespace Xcurse
{
    class Circle : public SingletonShape
    {
    public:
        Circle(Position midpoint, int radius, Size restraint = {0, 0});
        Circle(Size restraint);

        void rasterise() override;
        void rasterise_styled() override;
        void get_rasterised(std::vector<Position> &target) const override;
        void get_rasterised_styled(std::vector<Pixel> &target) const override;

        void set_radius(int r);
        void set_midpoint(Position p);
        void set_restraint(Size res);

    private:
        Position midpoint;
        int radius;
        Size restraint;
    };
}