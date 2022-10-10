#include "GenericShapeObject.h"

namespace Xcurse
{
    /**
     * @brief Rasterise the shape, and returns the reference of the result stored internally
     *
     */
    std::vector<Position> &GenericShapeObject::rasterise()
    {
        return m_rasterised_data;
    }
}