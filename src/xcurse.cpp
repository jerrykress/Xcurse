#include "xcurse.h"

namespace Xcurse
{
    // Initialise instance pointer
    Display *Display::m_instance = NULL;

    Display::Display()
    {
        get_size();
        m_screen = Screen(m_height, std::vector<char>(m_width, ' '));
    }

    Display *Display::get_display()
    {
        if (!m_instance)
            m_instance = new Display();
        return m_instance;
    }

    std::pair<int, int> Display::get_size()
    {
        struct winsize win;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
        m_width = win.ws_col;
        m_height = win.ws_row;
        return std::make_pair(win.ws_row, win.ws_col);
    }

    int Display::get_height() const
    {
        return m_height;
    }

    int Display::get_width() const
    {
        return m_width;
    }

    void Display::set_pixel(int x, int y, char c)
    {
        m_screen[y][x] = c;
    }

    bool Display::add_win(StaticWindow &w)
    {
        if (m_window_iterators.find(w.get_name()) != m_window_iterators.end())
        {
            throw std::runtime_error("Window with the same name already exists.");
            return false;
        }
        else
        {
            m_windows.emplace_back(w);
            m_window_iterators[w.get_name()] = --m_windows.end();
            return true;
        }
    }

    bool Display::add_win(FlexibleWindow &w)
    {
        return true;
    }

    bool Display::remove_win(std::string name)
    {
        return true;
    }

    void Display::refresh()
    {

        for (auto &win : m_windows)
        {
            win.draw(m_instance);
        }

        // output screen

        for (int j = 0; j < m_height; j++)
        {
            for (int i = 0; i < m_width; i++)
            {
                std::cout << m_screen[j][i];
            }
            std::cout << "\n";
        }
    }
} // namespace Xcurse
