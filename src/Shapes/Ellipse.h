#pragma once

#include "GenericShapeObject.h"

namespace Xcurse
{
    /**
     * @brief Ellipse shape
     *
     */
    class Ellipse : public GenericShapeObject
    {
    public:
        Ellipse();
        Ellipse(Position midpoint, int ra, int rb);

        std::vector<Position> &rasterise() override;

        void set_ra(int a);
        void set_rb(int b);
        void set_ra_rb(int a, int b);
        void set_midpoint(Position p);

        Position get_at_angle(float a);
        Position get_at_degree(float d);

    private:
        Position midpoint;
        int ra, rb;
    };
}