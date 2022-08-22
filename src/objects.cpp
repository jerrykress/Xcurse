#include "xcurse.h"

using namespace Xcurse;

GenericDisplayObject::GenericDisplayObject() {}

void GenericDisplayObject::draw()
{
    throw std::runtime_error("Object could not update content to display. Draw() method not implemented");
}

int GenericDisplayObject::get_height() const
{
    return m_size.height;
}

int GenericDisplayObject::get_width() const
{
    return m_size.width;
}

int GenericDisplayObject::get_weight() const
{
    return m_weight;
}

Position GenericDisplayObject::get_loc() const
{
    return m_loc;
}

Size GenericDisplayObject::get_size() const
{
    return m_size;
}

GenericWindowObject::GenericWindowObject() {}

void GenericWindowObject::set_title(std::wstring title)
{
    m_title = L" " + title + L" ";
}

void GenericWindowObject::set_override_win_style(bool b)
{
    override_win_style = b;
}

void GenericWindowObject::set_show_border(bool b)
{
    show_border = b;
}

void GenericWindowObject::set_show_titlebar(bool b)
{
    show_titlebar = b;
}

void GenericWindowObject::draw_border()
{
    // draw upper and lower borders
    for (int i = 1; i < m_size.width - 1; i++)
    {
        m_display_ptr->set_pixel(this, i, 0, Pixel(m_border[0], *this));
        m_display_ptr->set_pixel(this, i, m_size.height - 1, Pixel(m_border[1], *this));
    }
    // draw left and right borders
    for (int i = 1; i < m_size.height - 1; i++)
    {
        m_display_ptr->set_pixel(this, 0, i, Pixel(m_border[2], *this));
        m_display_ptr->set_pixel(this, m_size.width - 1, i, Pixel(m_border[3], *this));
    }
    // draw four corners of window
    m_display_ptr->set_pixel(this, 0, 0, Pixel(m_border[4], *this));
    m_display_ptr->set_pixel(this, m_size.width - 1, 0, Pixel(m_border[5], *this));
    m_display_ptr->set_pixel(this, 0, m_size.height - 1, Pixel(m_border[6], *this));
    m_display_ptr->set_pixel(this, m_size.width - 1, m_size.height - 1, Pixel(m_border[7], *this));
}

void GenericWindowObject::draw_titlebar()
{
    int title_len = m_title.size();
    int margin = m_size.width - title_len;
    if (margin < 2)
        return;

    int left_margin = margin / 2;

    for (int i = 0; i < title_len; i++)
    {
        m_display_ptr->set_pixel(this, i + left_margin, 0, Pixel(m_title[i], *this));
    }
}

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
    if (show_border)
        draw_border();
    if (show_titlebar)
        draw_titlebar();

    // draw the content of the window
    for (auto &data : m_windata)
    {
        m_display_ptr->set_pixel(this, data.first, data.second);
    }
}

Layout::Layout(std::string name, Direction direction, int weight) : _name(name), orientation(direction)
{
    m_weight = weight;
}

LayoutObjects *Layout::get_objects()
{
    return &m_objects;
}