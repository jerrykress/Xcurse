#include "GenericShapeObject.h"

namespace Xcurse
{
    /**
     * @brief Rasterise the shape into points
     *
     */
    void GenericShapeObject::rasterise() {}

    /**
     * @brief Get the raserised representation of the shape without style
     *
     * @return std::vector<Position>&
     */
    std::vector<Position> &GenericShapeObject::get_points()
    {
        return m_points;
    }

    /**
     * @brief Get the rasterised representation of the shape with style
     *
     * @return std::vector<Position>&
     */
    std::vector<Pixel> &GenericShapeObject::get_points_styled()
    {
        return m_pixels;
    }
}