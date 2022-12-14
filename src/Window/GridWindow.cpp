#include "GridWindow.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Grid Window:: Grid Window object
     *
     */
    GridWindow::GridWindow()
    {
        m_border = DEFAULT_WIN_BORDER;
        m_weight = 1;
    }

    /**
     * @brief Construct a new Grid Window:: Grid Window object
     *
     * @param weight
     * @param border
     */
    GridWindow::GridWindow(int weight, std::wstring border)
    {
        m_border = border;
        m_weight = weight;
    }

    /**
     * @brief Add a char to the window buffer
     *
     * @param x The x coord of the pixel to be set
     * @param y The y coord of the pixel to be set
     * @param c Char to be displayed
     * @param s Char style
     */
    void GridWindow::add_char(int x, int y, wchar_t c, Stylable s)
    {
        if (x >= 0 && x < get_width() && y >= 0 && y < get_height())
        {
            m_windata[Position{x, y}] = Pixel(c, s);
        }
    }

    /**
     * @brief Add a char to the window buffer
     *
     * @param p Position of the pixel to be set
     * @param c Char to be displayed
     * @param s Char style
     */
    void GridWindow::add_char(Position p, wchar_t c, Stylable s)
    {
        add_char(p.x, p.y, c, s);
    }

    /**
     * @brief Add a char to the window buffer
     *
     * @param x The x coord of the pixel to be set
     * @param y The y coord of the pixel to be set
     * @param c Pixel char
     * @param foreground foreground color
     * @param background background color
     * @param bold enable bold
     * @param underline enable underline
     * @param reversed enable reversed
     */
    void GridWindow::add_char(int x, int y, wchar_t c, Style foreground, Style background, bool bold, bool underline, bool reversed)
    {
        if (x >= 0 && x < get_width() && y >= 0 && y < get_height())
        {
            m_windata[Position{x, y}] = Pixel(c, Stylable(foreground, background, bold, underline, reversed));
        }
    }

    /**
     * @brief Add a set of chars to the window
     *
     * @param ps Vector of locations
     * @param c Char to be used at all locations
     * @param s Style for all chars
     */
    void GridWindow::add_chars(std::vector<Position> &ps, wchar_t c, Stylable s)
    {
        for (auto &p : ps)
        {
            add_char(p, c, s);
        }
    }

    /**
     * @brief Clear a pixel from the window buffer
     *
     * @param x The x coord of the pixel to be set
     * @param y The y coord of the pixel to be set
     *
     * @return If deletion was successful
     */
    bool GridWindow::clear_char(int x, int y)
    {
        if (auto it = m_windata.find(Position{x, y}); it != m_windata.end())
        {
            m_windata.erase(it);
            return true;
        }

        return false;
    }

    /**
     * @brief Empty the window buffer, remove all records
     *
     */
    void GridWindow::clean()
    {
        m_windata.clear();
    }

    /**
     * @brief Draw the entire window buffer to the display
     *
     */
    void GridWindow::draw()
    {
        if (m_show_border)
            draw_border();
        if (m_show_titlebar)
            draw_titlebar();

        // draw the content of the window
        for (auto &data : m_windata)
        {
            m_display_ptr->set_pixel(this, Position{data.first.x + (!m_disable_safe_area) * m_safe_area.left, m_size.height - 1 - data.first.y - (!m_disable_safe_area) * m_safe_area.down}, data.second);
        }
    }
}