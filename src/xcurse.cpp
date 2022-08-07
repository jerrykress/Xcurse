#include "xcurse.h"

using namespace Xcurse;
using namespace std::literals::chrono_literals;

// Initialise instance pointer
Display *Display::m_instance = NULL;

Display::Display()
{
    // setup basic attr
    get_size();
    m_screen = Screen(m_height, std::vector<char>(m_width, ' '));
    m_power = false;
    m_refresh_interval_ms = 1000;
    m_layout = new Layout("root");
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

void Display::set_pixel(int x, int y, char c)
{
    if (x > -1 && x < m_width && y > -1 && y < m_height - 1)
    {
        m_screen[y + 1][x] = c;
    }
}

bool Display::add_obj(std::string layout_name, std::string obj_name, GenericDisplayObject *o)
{
    if (auto target = m_obj_ptrs.find(layout_name); target != m_obj_ptrs.end() && typeid(target->second) == typeid(Layout *))
    {
        if (m_obj_ptrs.insert({obj_name, ObjInfo{o, target->second.obj_ptr}}).second)
        {
            static_cast<Layout *>(target->second.obj_ptr)->get_objects().emplace_back(o);
            return true;
        }
    }
    return false;
}

bool Display::remove_obj(std::string obj_name)
{
    // find target object to delete
    if (auto target = m_obj_ptrs.find(obj_name); target != m_obj_ptrs.end() && obj_name != "root")
    {
        if (auto parent_ptr = target->second.parent_ptr; parent_ptr != nullptr)
        {
            LayoutObjects &objects = static_cast<Layout *>(parent_ptr)->get_objects();

            for (auto it = objects.begin(); it != objects.end(); it++)
            {
                if (*it == target->second.obj_ptr)
                {
                    objects.erase(it);
                    return true;
                }
            }
        }
    }
    return false;
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
    if (m_width != win.ws_col || m_height != win.ws_row)
    {
        m_width = win.ws_col;
        m_height = win.ws_row;
        // update screen buffer
        m_screen.resize(m_height);
        for (auto &row : m_screen)
        {
            row.resize(m_width);
        }
        return true;
    }
    return false;
}

inline void Display::refresh_buffer()
{
    for (auto &obj_info : m_obj_ptrs)
    {
        obj_info.second.obj_ptr->draw();
    }
}

inline void Display::refresh_screen()
{
    for (int j = 0; j < m_height; j++)
    {
        for (int i = 0; i < m_width; i++)
        {
            std::cout << m_screen[j][i];
        }
        std::cout << "\n";
    }
}

void Display::refresh()
{
    while (m_power)
    {
        clear();
        // get size update status
        bool is_resize = update_size();
        // update layout
        if (is_resize)
            refreshLayout(*m_layout, 0, 0, m_height, m_width);
        // repaint windows to buffer
        refresh_buffer();
        // output screen
        refresh_screen();
        // wait for next refresh
        std::this_thread::sleep_for(std::chrono::milliseconds(m_refresh_interval_ms));
    }
}

void Display::set_refresh_interval(int ms)
{
    m_refresh_interval_ms = ms;
}

void Display::refreshLayout(Layout &layout, int x, int y, int max_height, int max_width)
{
    auto &objects = layout.get_objects();

    int total_units = std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, GenericDisplayObject *o)
                                      { return a + o->size_units; });

    if (layout.orientation == Horizontal)
    {
        for (auto &object : layout.get_objects())
        {
            object->m_height = max_height;
            object->m_width = max_width * object->size_units / total_units;

            if (typeid(object) == typeid(Layout))
            {
                refreshLayout(layout, x, y, max_height, object->m_width);
            }

            object->m_x = x += object->m_width;
            object->m_y = y;
        }
    }

    if (layout.orientation == Vertical)
    {
        for (auto &object : layout.get_objects())
        {
            object->m_width = max_width;
            object->m_height = max_height * object->size_units / total_units;

            if (typeid(object) == typeid(Layout))
            {
                refreshLayout(layout, x, y, object->m_height, max_width);
            }

            object->m_x = x;
            object->m_y = y += object->m_height;
        }
    }
}