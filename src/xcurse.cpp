#include "xcurse.h"

using namespace Xcurse;
using namespace std::literals::chrono_literals;

// Initialise instance pointer
Display *Display::m_instance = NULL;

Display::Display()
{
    get_size();
    m_screen = Screen(m_height, std::vector<char>(m_width, ' '));
    m_power = false;
    m_refresh_interval_ms = 1000;
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

GenericWindowObject *Display::get_window(std::string name)
{
    if (auto it = m_window_iterators.find(name); it != m_window_iterators.end())
    {
        return *it->second;
    }
    return nullptr;
}

void Display::set_pixel(int x, int y, char c)
{
    //! screen shift bug needs fixinng
    if (x > -1 && x < m_width && y > -1 && y < m_height - 1)
    {
        m_screen[y + 1][x] = c;
    }
}

bool Display::add_win(StaticWindow *w)
{
    if (m_window_iterators.find(w->get_name()) != m_window_iterators.end())
    {
        throw std::runtime_error("Window with the same name already exists.");
        return false;
    }
    else
    {
        m_windows.emplace_back(w);
        m_window_iterators[w->get_name()] = --m_windows.end();
        return true;
    }
}

bool Display::add_win(FlexibleWindow *w)
{
    return true;
}

bool Display::remove_win(std::string name)
{
    return true;
}

void Display::start()
{
    m_power = true;
    m_display_thread = std::thread(&Display::refresh, this);
}

void Display::poweroff()
{
    m_power = false;
    m_display_thread.join();
}

void Display::clear()
{
}

void Display::refresh()
{
    while (m_power)
    {
        for (auto &win : m_windows)
        {
            win->draw();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(m_refresh_interval_ms));
    }
}

void Display::set_refresh_interval(int ms)
{
    m_refresh_interval_ms = ms;
}
