#include "Display.h"

namespace Xcurse
{
    BaseDisplayObject::BaseDisplayObject()
    {
        m_visible = true;
    }

    void BaseDisplayObject::draw()
    {
        throw std::runtime_error("Object could not update content to display. Draw() method not implemented");
    }

    int BaseDisplayObject::get_height() const
    {
        return m_size.height;
    }

    int BaseDisplayObject::get_width() const
    {
        return m_size.width;
    }

    int BaseDisplayObject::get_weight() const
    {
        return m_weight;
    }

    Position BaseDisplayObject::get_loc() const
    {
        return m_loc;
    }

    Size BaseDisplayObject::get_size() const
    {
        return m_size;
    }

    void BaseDisplayObject::set_visible(bool b)
    {
        m_visible = b;
        m_display_ptr->update_layout();
    }

    void BaseDisplayObject::set_visible()
    {
        m_visible = !m_visible;
        m_display_ptr->update_layout();
    }
}