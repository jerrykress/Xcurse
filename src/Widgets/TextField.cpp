#include "TextField.h"

namespace Xcurse
{
    TextField::TextField()
    {
        _name = "Untitled";
        m_data = L" ";
        alignment = ALIGN_CENTER;
        m_size.fixed = true;
        m_size.height = 1;
        m_display_ptr = Display::get_display();
    }

    TextField::TextField(std::string name, const std::wstring &ws, Alignment a)
    {
        _name = name;
        m_data = ws;
        alignment = a;
        m_size.fixed = true;
        m_size.height = 1;
        m_display_ptr = Display::get_display();
    }

    void TextField::draw()
    {
        const int margin_all = m_size.width - m_data.size();
        const int margin_left = margin_all / 2;
        const int margin_right = margin_all - margin_left;

        switch (alignment)
        {
        case ALIGN_LEFT:
            // print content
            for (int i = 0; i < m_data.size(); i++)
            {
                m_display_ptr->set_pixel(this, i, 0, Pixel(m_data[i], *this));
            }
            // print rest of line
            for (int i = m_data.size(); i < m_size.width; i++)
            {
                m_display_ptr->set_pixel(this, i, 0, Pixel(L' ', *this));
            }
            break;

        case ALIGN_CENTER:
            // print left margin
            for (int i = 0; i < margin_left; i++)
            {
                m_display_ptr->set_pixel(this, i, 0, Pixel(L' ', *this));
            }
            // print content
            for (int i = 0; i < m_data.size(); i++)
            {
                m_display_ptr->set_pixel(this, i + margin_left, 0, Pixel(m_data[i], *this));
            }
            // print right margin
            for (int i = m_size.width - margin_right; i < m_size.width; i++)
            {
                m_display_ptr->set_pixel(this, i, 0, Pixel(L' ', *this));
            }
            break;

        case ALIGN_RIGHT:
            // print leading pixels
            for (int i = 0; i < margin_all; i++)
            {
                m_display_ptr->set_pixel(this, i, 0, Pixel(L' ', *this));
            }
            // print content
            for (int i = 0; i < m_data.size(); i++)
            {
                m_display_ptr->set_pixel(this, i + margin_all, 0, Pixel(m_data[i], *this));
            }
            break;

        default:
            break;
        }
    }
}