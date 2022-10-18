#pragma once

#include "GenericShapeObject.h"
#include <cmath>

namespace Xcurse
{
    /**
     * @brief Straight line shape
     *
     */
    class Line : public GenericShapeObject
    {
    public:
        Line();
        Line(Position begin, Position end);

        std::vector<Position> &rasterise() override;

        void set_begin(Position p);
        void set_end(Position p);
        void set_begin_end(Position a, Position b);

    private:
        Position begin, end;
    };
}