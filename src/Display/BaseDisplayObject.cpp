#include "Display.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Base Display Object:: Base Display Object object
     *
     */
    BaseDisplayObject::BaseDisplayObject()
    {
        m_visible = true;
    }

    /**
     * @brief Draw the content of the object onto the display buffer
     *
     */
    void BaseDisplayObject::draw()
    {
        throw std::runtime_error("Object could not update content to display. Draw() method not implemented");
    }

    /**
     * @brief Get the height of object
     *
     * @return int
     */
    int BaseDisplayObject::get_height() const
    {
        return m_size.height;
    }

    /**
     * @brief Get the width of object
     *
     * @return int
     */
    int BaseDisplayObject::get_width() const
    {
        return m_size.width;
    }

    /**
     * @brief Get the weight of object
     *
     * @return int
     */
    int BaseDisplayObject::get_weight() const
    {
        return m_weight;
    }

    /**
     * @brief Get location of object
     *
     * @return Position
     */
    Position BaseDisplayObject::get_loc() const
    {
        return m_loc;
    }

    /**
     * @brief Get size of object
     *
     * @return Size
     */
    Size BaseDisplayObject::get_size() const
    {
        return m_size;
    }

    /**
     * @brief Set the visibility of object
     *
     * @param b
     */
    void BaseDisplayObject::set_visible(bool b)
    {
        m_visible = b;
        m_display_ptr->update_layout();
    }

    /**
     * @brief Invert the visibility of object
     *
     */
    void BaseDisplayObject::set_visible()
    {
        m_visible = !m_visible;
        m_display_ptr->update_layout();
    }
}