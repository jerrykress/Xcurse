#include "Display.h"

namespace Xcurse
{
    Layout::Layout(std::string name, Direction direction, int weight) : _name(name), orientation(direction)
    {
        m_weight = weight;
    }

    /**
     * @brief Get the objects assigned to a layout
     *
     * @return A pointer to the collection of objects
     */
    LayoutObjects *Layout::get_objects()
    {
        return &m_objects;
    }
}