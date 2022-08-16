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
        m_buffer[i].resize(m_width, Pixel{L' ', ANSI_COLOR_RESET});
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
    for (int i = 0; i < m_height; i++)
    {
        std::fill(m_buffer[i].begin(), m_buffer[i].end(), Pixel{L' ', ANSI_COLOR_RESET});
    }
}

Window::Window(std::string name, int size, std::wstring border) : _name(name)
{
    m_border = border;
    size_units = size;
    m_buffer = Screen(1, std::vector<Pixel>(1, Pixel{L' ', ANSI_COLOR_RESET}));
    m_display_ptr = Display::get_display();
}

void Window::add_char(int x, int y, wchar_t c, PixelColor color)
{
    if (x > 0 && x < m_width - 1 && y > 0 && y < m_height - 1)
    {
        m_buffer[y][x].data = c;
        m_buffer[y][x].color = color;
    }
}

void Window::add_chars(const std::initializer_list<std::tuple<int, int, wchar_t, PixelColor>> &chars)
{
    for (auto &c : chars)
    {
        add_char(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c));
    }
}

void Window::add_str(int x, int y, const std::wstring &w_str, PixelColor color)
{
    for (wchar_t c : w_str)
    {
        add_char(x++, y, c, color);
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
        m_buffer.front()[i].data = m_border[0];
        m_buffer.back()[i].data = m_border[1];
    }
    // draw left and right
    for (int i = 1; i < m_height - 1; i++)
    {
        m_buffer[i].front().data = m_border[2];
        m_buffer[i].back().data = m_border[3];
    }
    // draw four corners
    m_buffer.front().front().data = m_border[4];
    m_buffer.front().back().data = m_border[5];
    m_buffer.back().front().data = m_border[6];
    m_buffer.back().back().data = m_border[7];
}

Layout::Layout(std::string name, Direction direction, int size) : _name(name), orientation(direction)
{
    size_units = size;
    m_buffer = Screen(1, std::vector<Pixel>(1, Pixel{L' ', ANSI_COLOR_RESET}));
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
