#include "xcurse.h"

namespace Xcurse
{
    void GenericWindowObject::add_char(int x, int y, char c)
    {
        if (x > -1 && x < m_width - 2 && y > -1 && y < m_height - 2)
        {
            m_buffer[m_height - 2 - y][x + 1] = c;
        }
    }

    void GenericWindowObject::add_str(int x, int y, std::string str)
    {
        for (char c : str)
        {
            add_char(x++, y, c);
        }
    }

    void GenericWindowObject::clear()
    {
        for (auto &row : m_buffer)
        {
            std::fill(row.begin(), row.end(), ' ');
        }
    }

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

        m_display_ptr = Display::get_display();

        m_buffer = Screen(m_height, std::vector<char>(m_width, ' '));

        for (int i = 1; i < m_width - 1; i++)
        {
            m_buffer[0][i] = m_border[0];
            m_buffer[m_height - 1][i] = m_border[1];
        }

        for (int i = 1; i < m_height - 1; i++)
        {
            m_buffer[i][0] = m_border[2];
            m_buffer[i][m_width - 1] = m_border[3];
        }

        m_buffer[0][0] = m_border[4];
        m_buffer[0][m_width - 1] = m_border[5];
        m_buffer[m_height - 1][0] = m_border[6];
        m_buffer[m_height - 1][m_width - 1] = m_border[7];
    }

    void StaticWindow::draw()
    {
        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++)
            {
                m_display_ptr->set_pixel(j + m_x, i + m_y, m_buffer[i][j]);
            }
        }
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

    void FlexibleWindow::draw()
    {
    }
}