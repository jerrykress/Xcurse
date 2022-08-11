#include "xcurse.h"

using namespace Xcurse;
using namespace std::literals::chrono_literals;

// Initialise instance pointer
Display *Display::m_instance = NULL;

Display::Display()
{
    // setup basic attr
    update_size();
    m_screen = Screen(MAX_BUF_H, std::vector<char>(MAX_BUF_W, ' '));
    m_power = false;
    m_refresh_interval = 100;
    m_layout = new Layout("root", Vertical, 1);
    m_layout->parent_ptr = nullptr;
    m_obj_ptrs.emplace("root", m_layout);
}

Display *Display::get_display()
{
    if (!m_instance)
        m_instance = new Display();
    return m_instance;
}

bool Display::update_size()
{
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);

    if (m_width != win.ws_col || m_height != win.ws_row)
    {
        m_width = win.ws_col;
        m_height = win.ws_row;

        return true;
    }
    return false;
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
    // find if the target layout exists
    if (auto layout_pair_it = m_obj_ptrs.find(layout_name); layout_pair_it != m_obj_ptrs.end() && typeid(*(layout_pair_it->second)) == typeid(Layout))
    {
        // insert the object into the global object record table if no object with the same exists
        if (m_obj_ptrs.insert({obj_name, o}).second)
        {
            // assign parent to incoming object
            o->parent_ptr = layout_pair_it->second;
            // add object pointer to parent's record
            static_cast<Layout *>(layout_pair_it->second)->get_objects()->emplace_back(o);
            return true;
        }
    }
    return false;
}

bool Display::remove_obj(std::string obj_name)
{
    // find if the object exists and the object must not be root layout
    if (auto obj_pair_it = m_obj_ptrs.find(obj_name); obj_pair_it != m_obj_ptrs.end() && obj_name != "root")
    {
        LayoutObjects &objects = *static_cast<Layout *>(obj_pair_it->second->parent_ptr)->get_objects();

        for (auto it = objects.begin(); it != objects.end(); it++)
        {
            if (*it == obj_pair_it->second)
            {
                objects.erase(it);
                return true;
            }
        }
    }
    return false;
}

GenericDisplayObject *Display::get_obj(std::string name)
{
    if (m_obj_ptrs.count(name))
    {
        return m_obj_ptrs[name];
    }
    return nullptr;
}

void Display::power_on()
{
    if (!m_power)
    {
        m_power = true;
        // enter alternate buffer
        std::cout << "\e[?47h" << std::endl;
        // hide cursor
        std::cout << "\e[?25l" << std::endl;
        system("stty -icanon");
        system("stty -echo");
        system("echo \"\e[?1003h\"");
        // init refresh
        refreshLayout(m_layout, 0, 0, m_height, m_width);
        m_refresh_thread = std::thread(&Display::refresh, this);
        m_mouse_in_thread = std::thread(&Display::mouse_handler, this);
    }
}

void Display::power_off()
{
    if (m_power)
    {
        m_power = false;
        m_refresh_thread.join();
        system("echo \"\e[?1000l\"");
        m_mouse_in_thread.join();
        system("stty echo");
        // enable cursor
        std::cout << "\e[?25h" << std::endl;
        // leave alternate buffer
        std::cout << "\e[?47l" << std::endl;
    }
}

inline void Display::clear_terminal()
{
    std::cout << "\x1B[2J\x1B[H";
}

void Display::clear_buffer()
{
    for (auto &row : m_screen)
    {
        std::fill(row.begin(), row.end(), ' ');
    }
}

// print out the content of the screen to the terminal
inline void Display::output_screen()
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
        clear_buffer();
        // get size update status
        bool is_resize = update_size();
        // update layout
        refreshLayout(m_layout, 0, 0, m_height, m_width);
        // output screen
        output_screen();
        // wait for next refresh
        std::this_thread::sleep_for(std::chrono::milliseconds(m_refresh_interval));
    }
}

void Display::set_refresh_interval(int ms)
{
    m_refresh_interval = ms;
}

void Display::refreshLayout(Layout *layout, int x, int y, int max_height, int max_width)
{

    LayoutObjects &objects = *(layout->get_objects());

    int total_units = std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, GenericDisplayObject *o)
                                      { return a + o->size_units; });

    if (layout->orientation == Horizontal)
    {
        for (auto object : objects)
        {
            object->m_height = max_height;
            object->m_width = std::floor(1.0f * max_width * object->size_units / total_units);
            object->resize(object->m_width, object->m_height);

            if (typeid(*object) == typeid(Layout))
            {
                refreshLayout(static_cast<Layout *>(object), x, y, max_height, object->m_width);
            }

            object->refresh_buffer();
            object->draw();

            object->m_x = x;
            object->m_y = y;
            x += object->m_width;
        }
    }

    if (layout->orientation == Vertical)
    {
        for (auto object : objects)
        {
            object->m_width = max_width;
            object->m_height = std::floor(1.0f * max_height * object->size_units / total_units);
            object->resize(object->m_width, object->m_height);

            if (typeid(*object) == typeid(Layout))
            {
                refreshLayout(static_cast<Layout *>(object), x, y, object->m_height, max_width);
            }

            object->refresh_buffer();
            object->draw();

            object->m_x = x;
            object->m_y = y;
            y += object->m_height;
        }
    }
}

void Display::mouse_handler()
{
    //! do something with the token
    char *token[17];
    while (m_power)
    {
        fread(token, 1, 16, stdin);
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
        std::cout << obj.second->m_x << ", "
                  << obj.second->m_y << ", "
                  << obj.second->m_width << ", "
                  << obj.second->m_height << ", "
                  << obj.second->size_units << "\n"
                  << std::endl;
    }
}