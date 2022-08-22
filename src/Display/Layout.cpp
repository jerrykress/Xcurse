#include "Display.h"

namespace Xcurse
{
    Layout::Layout(std::string name, Direction direction, int weight) : _name(name), orientation(direction)
    {
        m_weight = weight;
    }

    LayoutObjects *Layout::get_objects()
    {
        return &m_objects;
    }
}