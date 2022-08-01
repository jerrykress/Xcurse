#include "xcurse.h"

namespace Xcurse
{
    StaticWindow::StaticWindow(std::string name, int x, int y, int width, int height, std::string border)
    {
        if (x < 0 || y < 0 || width < 0 || height < 0 || name.empty() || border.size() != 8)
        {
            throw std::invalid_argument("Incorrect parameters when constructing a StaticWindow.");
        }

        m_name = name;
        m_x = x;
        m_y = y;
        m_width = std::max(width, 3);
        m_height = std::max(height, 3);
        m_border = border;
    }

    void StaticWindow::draw(Display *d)
    {
        // draw edges
        for (int i = 1; i < m_width - 1; i++)
        {
            d->set_pixel(i + m_x, m_y, m_border[0]);
            d->set_pixel(i + m_x, m_y + m_height - 1, m_border[1]);
        }
        for (int i = 1; i < m_height - 1; i++)
        {
            d->set_pixel(m_x, i + m_y, m_border[2]);
            d->set_pixel(m_x + m_width - 1, i + m_y, m_border[3]);
        }
        // draw corners
        d->set_pixel(m_x, m_y, m_border[4]);
        d->set_pixel(m_x + m_width - 1, m_y, m_border[5]);
        d->set_pixel(m_x, m_y + m_height - 1, m_border[6]);
        d->set_pixel(m_x + m_width - 1, m_y + m_height - 1, m_border[7]);
    }

    FlexibleWindow::FlexibleWindow(std::string name, int x, int y, int w_percent, int h_percent, std::string border)
    {
        if (x < 0 || y < 0 || w_percent < 1 || h_percent < 1 || name.empty() || border.size() != 8)
        {
            throw std::invalid_argument("Incorrect parameters when constructing a FlexibleWindow.");
        }

        m_name = name;
        m_x = x;
        m_y = y;
        m_w_percent = w_percent;
        m_h_percent = h_percent;
        m_border = border;
    }

    int FlexibleWindow::get_w_percent() const
    {
        return m_w_percent;
    }

    int FlexibleWindow::get_h_percent() const
    {
        return m_h_percent;
    }

    void FlexibleWindow::set_w_percent(int wp)
    {
        m_w_percent = wp;
    }

    void FlexibleWindow::set_h_percent(int hp)
    {
        m_h_percent = hp;
    }

    void FlexibleWindow::draw(Display *d)
    {
    }
}