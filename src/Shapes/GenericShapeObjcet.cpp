#include "GenericShapeObject.h"

namespace Xcurse
{
    /**
     * @brief Rasterise the shape into points
     *
     */
    void GenericShapeObject::rasterise() {}

    /**
     * @brief Rasterise the shape into pixels with style
     *
     */
    void GenericShapeObject::rasterise_styled() {}

    /**
     * @brief Get the raserised representation of the shape without style
     *
     */
    void GenericShapeObject::get_rasterised(std::vector<Position> &target)
    {
        target = std::vector<Position>{};
    }

    /**
     * @brief Get the rasterised representation of the shape with style
     *
     */
    void GenericShapeObject::get_rasterised_styled(std::vector<Pixel> &target)
    {
        target = std::vector<Pixel>{};
    }
}