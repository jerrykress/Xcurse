#include "xcurse.h"
#include "logger.h"
using namespace Xcurse;
using namespace std::literals::chrono_literals;

Console::Logger *lg = Console::Logger::get_logger();
// Initialise instance pointer
Display *Display::m_instance = NULL;

Display::Display()
{
    // setup basic attr
    get_size();
    m_screen = Screen(m_height, std::vector<char>(m_width, ' '));
    m_power = false;
    m_refresh_interval_ms = 1000;
    m_layout = new Layout("root", Vertical, 1);
    m_obj_ptrs.emplace("root", ObjInfo{m_layout, nullptr});
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
    // lg->info("enter set pixel " + std::to_string(x) + " " + std::to_string(y) + " " + c);
    // lg->info("w/h: " + std::to_string(m_width) + " " + std::to_string(m_height));
    if (x > -1 && x < m_width && y > -1 && y < m_height - 1)
    {
        // lg->info("setting pixel");
        m_screen[y + 1][x] = c;
    }
}

bool Display::add_obj(std::string layout_name, std::string obj_name, GenericDisplayObject *o)
{
    if (auto target = m_obj_ptrs.find(layout_name); target != m_obj_ptrs.end() && typeid(*(target->second.obj_ptr)) == typeid(Layout))
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
        // init refresh
        refreshLayout(m_layout, 0, 0, m_height, m_width);
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
    // lg->info("old win size is: " + std::to_string(m_height) + ", " + std::to_string(m_width));
    // lg->info("new win size is: " + std::to_string(win.ws_row) + ", " + std::to_string(win.ws_col));
    if (m_width != win.ws_col || m_height != win.ws_row)
    {
        m_width = win.ws_col;
        m_height = win.ws_row;
        // lg->info("now win size is: " + std::to_string(m_height) + ", " + std::to_string(m_width));
        // update screen buffer
        // m_screen.resize(m_height);
        // for (auto &row : m_screen)
        // {
        //     row.resize(m_width);
        // }
        return true;
    }
    return false;
}

// draw from object buffers to main screen buffer
inline void Display::refresh_buffer()
{
    for (auto &obj_info : m_obj_ptrs)
    {
        lg->info("refreshing buffer" + (obj_info.first));
        std::cout << "terminal size: " << m_width << "x" << m_height << std::endl;
        obj_info.second.obj_ptr->refresh_buffer();
        lg->info("drawing");
        obj_info.second.obj_ptr->draw();
    }
}

// print out the content of the screen to the terminal
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
    lg->info("refreshing");
    while (m_power)
    {
        clear();
        // get size update status
        bool is_resize = update_size();
        // update layout
        if (is_resize)
            refreshLayout(m_layout, 0, 0, m_height, m_width);
        // m_instance->status();
        // std::cin.get();
        // repaint windows to buffer
        lg->info("refresh buffer...");
        refresh_buffer();
        // output screen
        lg->info("refresh screen...");
        refresh_screen();
        // wait for next refresh
        std::this_thread::sleep_for(std::chrono::milliseconds(m_refresh_interval_ms));
    }
}

void Display::set_refresh_interval(int ms)
{
    m_refresh_interval_ms = ms;
}

void Display::refreshLayout(Layout *layout, int x, int y, int max_height, int max_width)
{
    lg->info("refreshing layout: " + std::to_string(max_height) + std::to_string(max_width));
    auto &objects = layout->get_objects();

    lg->info("got objects");
    int total_units = std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, GenericDisplayObject *o)
                                      { return a + o->size_units; });
    lg->info("size total: " + std::to_string(total_units));

    if (layout->orientation == Horizontal)
    {
        lg->info("hor layout");
        for (auto &object : layout->get_objects())
        {
            object->m_height = max_height;
            object->m_width = max_width * object->size_units / total_units;
            object->resize(object->m_width, object->m_height);

            if (typeid(*object) == typeid(Layout))
            {
                refreshLayout(static_cast<Layout *>(object), x, y, max_height, object->m_width);
            }

            object->m_x = x;
            object->m_y = y;
            x += object->m_width;
        }
    }

    if (layout->orientation == Vertical)
    {
        lg->info("ver layout");
        for (auto &object : layout->get_objects())
        {
            lg->info("processing object" + std::to_string(object->size_units));
            object->m_width = max_width;
            object->m_height = max_height * object->size_units / total_units;
            object->resize(object->m_width, object->m_height);
            lg->info("actual obj size" + std::to_string(object->m_buffer.size()) + "x" + std::to_string(object->m_buffer.front().size()));

            if (typeid(*object) == typeid(Layout))
            {
                refreshLayout(static_cast<Layout *>(object), x, y, object->m_height, max_width);
            }

            object->m_x = x;
            object->m_y = y;
            y += object->m_height;
        }
    }
}

void Display::status() const
{
    std::cout << "terminal size: " << m_width << "x" << m_height << std::endl;
    std::cout << "m_obj_ptr size: " << m_obj_ptrs.size() << std::endl;
    std::cout << "m_layout size: " << m_layout->m_objects.size() << std::endl;

    for (auto &obj : m_obj_ptrs)
    {
        std::cout << obj.first << ": ";
        std::cout << obj.second.obj_ptr->m_x << ", "
                  << obj.second.obj_ptr->m_y << ", "
                  << obj.second.obj_ptr->m_width << ", "
                  << obj.second.obj_ptr->m_height << ", "
                  << obj.second.obj_ptr->size_units << "\n"
                  << std::endl;
    }
}