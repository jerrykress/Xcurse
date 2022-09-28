#include "GridWindow.h"

namespace Xcurse
{

    GridWindow::GridWindow(std::string name, int weight, std::wstring border)
    {
        _name = name;
        m_border = border;
        m_weight = weight;
        m_display_ptr = Display::get_display();
    }

    void GridWindow::add_char(int x, int y, wchar_t c, Style foreground, Style background, bool bold, bool underline, bool reversed)
    {
        if (x > 0 && x < m_size.width - 1 && y > 0 && y < m_size.height - 1)
        {
            m_windata[Position{x, y}] = Pixel(c, Stylable(foreground, background, bold, underline, reversed));
        }
    }

    bool GridWindow::clear_char(int x, int y)
    {
        if (auto it = m_windata.find(Position{x, y}); it != m_windata.end())
        {
            m_windata.erase(it);
            return true;
        }
        return false;
    }

    void GridWindow::clean()
    {
        m_windata.clear();
    }

    void GridWindow::draw()
    {
        if (m_show_border)
            draw_border();
        if (m_show_titlebar)
            draw_titlebar();

        // draw the content of the window
        for (auto &data : m_windata)
        {
            m_display_ptr->set_pixel(this, data.first, data.second);
        }
    }
}