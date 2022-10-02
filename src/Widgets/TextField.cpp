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
        const int margin_left = (m_size.width - m_data.size()) / 2;

        for (int i = 0; i < m_data.size(); i++)
        {
            m_display_ptr->set_pixel(this, i, 0, Pixel(m_data[i], Stylable(TEXT_COLOR_BRIGHT_WHITE, BACKGROUND_COLOR_BLUE, false, false, false)));
        }
    }
}