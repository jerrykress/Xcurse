#include "CompositeShape.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Composite Shape:: Composite Shape object
     *
     */
    CompositeShape::CompositeShape() {}

    /**
     * @brief Add shape to composite
     *
     * @param name name of shape
     * @param o shape object
     */
    void CompositeShape::add_shape(std::string name, GenericShapeObject o)
    {
        if (m_shapes.find(name) == m_shapes.end())
        {
            m_shapes[name] = o;
        }
    }

    /**
     * @brief Remove a shape from composite
     *
     * @param name name of object
     */
    void CompositeShape::rm_shape(std::string name)
    {
        m_shapes.erase(name);
    }

    /**
     * @brief Get a copy of a shape, returns empty shape if not found
     *
     * @param name name of object
     * @return GenericShapeObject
     */
    GenericShapeObject CompositeShape::get_shape(std::string name)
    {
        if (auto it = m_shapes.find(name); it != m_shapes.end())
        {
            return it->second;
        }
        // empty object
        return GenericShapeObject();
    }

    /**
     * @brief Rasterise all shapes
     *
     * @return std::vector<Position>&
     */
    std::vector<Position> &CompositeShape::rasterise()
    {
        // TODO:
        return m_rasterised_data;
    }

    GenericShapeObject &CompositeShape::operator[](std::string name)
    {
        return m_shapes[name];
    }

}