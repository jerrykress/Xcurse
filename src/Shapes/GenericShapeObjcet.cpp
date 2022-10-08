#include "GenericShapeObject.h"

namespace Xcurse
{
    /**
     * @brief Rasterise the shape into points and store internally
     *
     */
    void GenericShapeObject::rasterise() {}

    /**
     * @brief Rasterise the shape into pixels with style and store internally
     *
     */
    void GenericShapeObject::rasterise_styled() {}

    /**
     * @brief Get the raserised representation of the shape without style, directly output to target container. Does not store internally
     *
     */
    void GenericShapeObject::get_rasterised(std::vector<Position> &target) const
    {
        target = std::vector<Position>{};
    }

    /**
     * @brief Get the rasterised representation of the shape with style, directly store output to target container. Does not store internally
     *
     */
    void GenericShapeObject::get_rasterised_styled(std::vector<Pixel> &target) const
    {
        target = std::vector<Pixel>{};
    }
}