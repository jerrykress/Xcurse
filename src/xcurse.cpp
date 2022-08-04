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

Display::Size Display::get_size()
{
    update_size();
    return Display::Size{m_width, m_height};
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
    if (auto it = m_windows.find(name); it != m_windows.end())
    {
        return it->second;
    }
    return nullptr;
}

void Display::set_pixel(int x, int y, char c)
{
    if (x > -1 && x < m_width && y > -1 && y < m_height - 1)
    {
        m_screen[y + 1][x] = c;
    }
}

bool Display::add_win(StaticWindow *w)
{
    if (m_windows.find(w->get_name()) != m_windows.end())
    {
        throw std::runtime_error("Window with the same name already exists.");
        return false;
    }
    else
    {
        m_windows[w->get_name()] = w;
        return true;
    }
}

bool Display::add_win(FlexibleWindow *w)
{
    return true;
}

bool Display::remove_win(std::string name)
{
    if (auto it = m_windows.find(name); it != m_windows.end())
    {
        // free window memory
        delete it->second;
        m_windows.erase(name);
        return true;
    }
    else
    {
        return false;
    }
}

void Display::start()
{
    if (!m_power)
    {
        m_power = true;
        // enter alternate buffer
        std::cout << "\e[?47h" << std::endl;
        // hide cursor
        std::cout << "\e[?25l" << std::endl;
        m_display_thread = std::thread(&Display::refresh, this);
    }
}

void Display::poweroff()
{
    if (m_power)
    {
        m_power = false;
        m_display_thread.join();
        // enable cursor
        std::cout << "\e[?25h" << std::endl;
        // leave alternate buffer
        std::cout << "\e[?47l" << std::endl;
    }
}

void Display::clear()
{
    for (auto &row : m_screen)
    {
        std::fill(row.begin(), row.end(), ' ');
    }
}

bool Display::update_size()
{
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    bool updated = m_width != win.ws_col || m_height != win.ws_row;
    m_width = win.ws_col;
    m_height = win.ws_row;
    return updated;
}

void Display::refresh()
{
    while (m_power)
    {
        clear();
        // get size update status
        bool is_resize = update_size();
        // redraw windows
        for (auto &win_info : m_windows)
        {
            win_info.second->draw(is_resize);
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
