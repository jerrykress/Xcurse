#include "xcurse.h"
#include "logger.h"

using namespace Xcurse;

void GenericDisplayObject::draw()
{
    return;
}

void GenericDisplayObject::resize(int w, int h)
{
    m_buffer.resize(m_height);
    for (int i = 0; i < m_height; i++)
    {
        m_buffer[i].resize(m_width, ' ');
    }
}

void GenericDisplayObject::refresh_buffer()
{
    return;
}

void GenericDisplayObject::clear_buffer()
{
    for (auto &row : m_buffer)
    {
        std::fill(row.begin(), row.end(), ' ');
    }
}

Window::Window(std::string name, int size, std::string border) : _name(name)
{
    m_border = border;
    size_units = size;
    m_buffer = Screen(1, std::vector<char>(1, ' '));
    m_display_ptr = Display::get_display();
}

void Window::draw()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            m_display_ptr->set_pixel(x + m_x, y + m_y, m_buffer[y][x]);
        }
    }
}

void Window::refresh_buffer()
{
    // draw top and bottom
    for (int i = 1; i < m_width - 1; i++)
    {
        m_buffer.front()[i] = m_border[0];
        m_buffer.back()[i] = m_border[1];
    }
    // draw left and right
    for (int i = 1; i < m_height - 1; i++)
    {
        m_buffer[i].front() = m_border[2];
        m_buffer[i].back() = m_border[3];
    }
    // draw four corners
    m_buffer.front().front() = m_border[4];
    m_buffer.front().back() = m_border[5];
    m_buffer.back().front() = m_border[6];
    m_buffer.back().back() = m_border[7];
}

Layout::Layout(std::string name, Direction direction, int size) : _name(name), orientation(direction)
{
    size_units = size;
    m_buffer = Screen(1, std::vector<char>(1, ' '));
    // m_display_ptr = Display::get_display();
}

void Layout::refresh_buffer()
{
    return;
}
