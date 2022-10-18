#pragma once

#include "GenericShapeObject.h"

namespace Xcurse
{
    /**
     * @brief Circle shape
     *
     */
    class Circle : public GenericShapeObject
    {
    public:
        Circle();
        Circle(Position midpoint, int radius);

        std::vector<Position> &rasterise() override;

        void set_radius(int r);
        void set_midpoint(Position p);

        Position get_at_angle(float a);
        Position get_at_degree(float d);

    private:
        Position midpoint;
        int radius;
    };
}