#include "GenericWindowObject.h"

namespace Xcurse
{
    GenericWindowObject::GenericWindowObject() {}

    /**
     * @brief Get the height of the usable area
     *
     * @return Height of inner area, or height of window if safe area is disabled
     */
    int GenericWindowObject::get_height() const
    {
        return m_disable_safe_area ? m_size.height : (m_size.height - 2);
    }

    /**
     * @brief Get the width of the usable area
     *
     * @return Width of inner area, or width of window if safe area is disabled
     */
    int GenericWindowObject::get_width() const
    {
        return m_disable_safe_area ? m_size.width : (m_size.width - 2);
    }

    /**
     * @brief Get the size of usable area
     *
     * @return Usable size of inner area, or size of window if safe area is disabled
     */
    Size GenericWindowObject::get_size() const
    {
        return m_disable_safe_area ? m_size : Size{m_size.width - 2, m_size.height - 2};
    }

    /**
     * @brief Set the text displayed on the titlebar
     *
     * @param title
     */
    void GenericWindowObject::set_title(std::wstring title)
    {
        m_title = title.empty() ? L" " : title;
    }

    /**
     * @brief Let the style of border override the style of the entire window
     *
     * @param b
     */
    void GenericWindowObject::set_override_win_style(bool b)
    {
        m_override_win_style = b;
    }

    /**
     * @brief Whether to use or disable the safe area (border)
     *
     * @param b
     */
    void GenericWindowObject::use_safe_area(bool b)
    {
        m_disable_safe_area = b;
    }

    /**
     * @brief Whether to display the border of a window
     *
     * @param b
     */
    void GenericWindowObject::set_show_border(bool b)
    {
        m_show_border = b;
    }

    /**
     * @brief Whether to display the titlebar of a window
     *
     * @param b
     */
    void GenericWindowObject::set_show_titlebar(bool b)
    {
        m_show_titlebar = b;
    }

    /**
     * @brief Render the border of a window to display
     *
     */
    void GenericWindowObject::draw_border()
    {
        // draw upper and lower borders
        for (int i = 1; i < m_size.width - 1; i++)
        {
            m_display_ptr->set_pixel(this, i, 0, Pixel(m_border[0], *this));
            m_display_ptr->set_pixel(this, i, m_size.height - 1, Pixel(m_border[1], *this));
        }
        // draw left and right borders
        for (int i = 1; i < m_size.height - 1; i++)
        {
            m_display_ptr->set_pixel(this, 0, i, Pixel(m_border[2], *this));
            m_display_ptr->set_pixel(this, m_size.width - 1, i, Pixel(m_border[3], *this));
        }
        // draw four corners of window
        m_display_ptr->set_pixel(this, 0, 0, Pixel(m_border[4], *this));
        m_display_ptr->set_pixel(this, m_size.width - 1, 0, Pixel(m_border[5], *this));
        m_display_ptr->set_pixel(this, 0, m_size.height - 1, Pixel(m_border[6], *this));
        m_display_ptr->set_pixel(this, m_size.width - 1, m_size.height - 1, Pixel(m_border[7], *this));
    }

    /**
     * @brief Render the titlebar of a window to display
     *
     */
    void GenericWindowObject::draw_titlebar()
    {
        int title_len = m_title.size();
        int margin = m_size.width - title_len;
        if (margin < 2)
            return;

        // calculate margins on the left of title
        int left_margin = margin / 2;

        // if title is not empty then render the text with spaces
        if (m_title != L" ")
        {
            for (int i = 0; i < title_len; i++)
            {
                m_display_ptr->set_pixel(this, i + left_margin, 0, Pixel(m_title[i], *this));
            }
        }
    }
}