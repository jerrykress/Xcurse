#include "GenericWindowObject.h"

namespace Xcurse
{
    GenericWindowObject::GenericWindowObject() {}

    void GenericWindowObject::set_title(std::wstring title)
    {
        m_title = title.empty() ? L" " : title;
    }

    void GenericWindowObject::set_override_win_style(bool b)
    {
        override_win_style = b;
    }

    void GenericWindowObject::set_show_border(bool b)
    {
        show_border = b;
    }

    void GenericWindowObject::set_show_titlebar(bool b)
    {
        show_titlebar = b;
    }

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