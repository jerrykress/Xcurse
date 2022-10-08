#pragma once

#include "../Common/Common.h"

namespace Xcurse
{
    class GenericShapeObject
    {
    public:
        virtual void rasterise();
        virtual void rasterise_styled();
        virtual void get_rasterised(std::vector<Position> &target) const;
        virtual void get_rasterised_styled(std::vector<Pixel> &target) const;

    private:
        std::vector<Position> m_points;
        std::vector<Pixel> m_pixels;
    };
}