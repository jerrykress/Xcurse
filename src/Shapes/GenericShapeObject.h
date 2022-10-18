#pragma once

#include "../Common/Common.h"

namespace Xcurse
{
    /**
     * @brief Base template of all shape objects
     *
     */
    class GenericShapeObject : public Stylable
    {
    public:
        virtual std::vector<Position> &rasterise();

    protected:
        std::vector<Position> m_rasterised_data;
    };
}