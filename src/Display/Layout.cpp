#include "Display.h"

namespace Xcurse
{
    Layout::Layout(Direction direction, int weight) : orientation(direction)
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