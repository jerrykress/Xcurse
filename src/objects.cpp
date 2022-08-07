#include "xcurse.h"

using namespace Xcurse;

void GenericDisplayObject::draw()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            m_display_ptr->set_pixel(x + m_x, y + m_y, m_buffer[y][x]);
        }
    }
}

void GenericDisplayObject::resize(int w, int h)
{
    m_buffer.resize(m_height);
    for (auto &row : m_buffer)
    {
        row.resize(m_width);
    }
}

void GenericDisplayObject::refresh_buffer()
{
}

void Window::refresh_buffer()
{
    // draw border
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

Layout::Layout(std::string name) : _name(name) {}

Window::Window(std::string name, int size, std::string border)
{
}