#pragma once

#include "../Common/Common.h"

namespace Xcurse
{
    class GenericShapeObject
    {
    public:
        virtual void rasterise();
        virtual void rasterise_styled();
        virtual std::vector<Position> &get_points();
        virtual std::vector<Pixel> &get_points_styled();

    private:
        std::vector<Position> m_points;
        std::vector<Pixel> &m_pixels;
    };
}