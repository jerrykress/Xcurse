#include "TextField.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Text Field:: Text Field object
     *
     */
    TextField::TextField()
    {
        m_data = L" ";
        alignment = ALIGN_CENTER;
        m_size.fixed = true;
        m_size.height = 1;
    }

    /**
     * @brief Construct a new Text Field:: Text Field object
     *
     * @param ws text
     * @param a alignment
     */
    TextField::TextField(const std::wstring &ws, Alignment a)
    {
        m_data = ws;
        alignment = a;
        m_size.fixed = true;
        m_size.height = 1;
    }

    /**
     * @brief Construct a new Text Field:: Text Field object
     *
     * @param s text
     * @param a alignment
     */
    TextField::TextField(const std::string &s, Alignment a)
    {
        m_data = to_wstring(s);
        alignment = a;
        m_size.fixed = true;
        m_size.height = 1;
    }

    /**
     * @brief Set text content
     *
     * @param ws text
     */
    void TextField::set_data(const std::wstring &ws)
    {
        m_data = ws;
    }

    /**
     * @brief Set text content
     *
     * @param s text
     */
    void TextField::set_data(const std::string &s)
    {
        m_data = to_wstring(s);
    }

    /**
     * @brief render to display
     *
     */
    void TextField::draw()
    {
        const int margin_lr = m_size.width - m_data.size();
        const int margin_l = margin_lr / 2;
        const int margin_r = margin_lr - margin_l;

        switch (alignment)
        {
        case ALIGN_LEFT:
            m_display_ptr->set_pixels(this, 0, 0, m_data, *this);
            m_display_ptr->set_pixels(this, m_data.size(), 0, std::wstring(margin_lr, ' '), *this);
            break;

        case ALIGN_CENTER:
            m_display_ptr->set_pixels(this, 0, 0, std::wstring(margin_l, ' '), *this);
            m_display_ptr->set_pixels(this, margin_l, 0, m_data, *this);
            m_display_ptr->set_pixels(this, margin_l + m_data.size(), 0, std::wstring(margin_r, ' '), *this);
            break;

        case ALIGN_RIGHT:
            m_display_ptr->set_pixels(this, 0, 0, std::wstring(margin_lr, ' '), *this);
            m_display_ptr->set_pixels(this, margin_lr, 0, m_data, *this);
            break;

        default:
            break;
        }
    }
}