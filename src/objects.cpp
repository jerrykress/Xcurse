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
        m_buffer[i].resize(m_width, L' ');
    }
}

int GenericDisplayObject::get_height() const
{
    return m_height;
}

int GenericDisplayObject::get_width() const
{
    return m_width;
}

int GenericDisplayObject::get_size() const
{
    return size_units;
}

Position GenericDisplayObject::get_pos() const
{
    return Position{m_x, m_y};
}

void GenericDisplayObject::refresh_buffer()
{
    return;
}

void GenericDisplayObject::clear_buffer()
{
    for (auto &row : m_buffer)
    {
        std::fill(row.begin(), row.end(), L' ');
    }
}

Window::Window(std::string name, int size, std::wstring border) : _name(name)
{
    m_border = border;
    size_units = size;
    m_buffer = Screen(1, std::vector<wchar_t>(1, L' '));
    m_display_ptr = Display::get_display();
}

void Window::add_char(int x, int y, wchar_t c)
{
    if (x > 0 && x < m_width - 1 && y > 0 && y < m_height - 1)
    {
        m_buffer[y][x] = c;
    }
}

void Window::add_chars(const std::initializer_list<std::tuple<int, int, wchar_t>> &chars)
{
    for (auto &c : chars)
    {
        add_char(std::get<0>(c), std::get<1>(c), std::get<2>(c));
    }
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
    m_buffer = Screen(1, std::vector<wchar_t>(1, L' '));
}

LayoutObjects *Layout::get_objects()
{
    return &m_objects;
}

Text::Text(std::string data) : m_data(data)
{
    m_height = 1;
    m_width = data.size();
}
