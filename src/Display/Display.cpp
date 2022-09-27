#include "Display.h"

namespace Xcurse
{

    using namespace std::literals::chrono_literals;

    // Initialise instance pointer
    Display *Display::m_instance = NULL;

    // overload array subscript operator
    BaseDisplayObject *&Display::operator[](std::string name)
    {
        return m_obj_ptrs[name];
    }

    /**
     * @brief Construct a new Display:: Display object
     *
     */
    Display::Display()
    {
        // setup basic attr
        update_size();
        m_screen = Screen(MAX_BUF_H, std::vector<Pixel>(MAX_BUF_W, Pixel()));
        m_power = false;
        m_power_off_all = true;
        m_refresh_interval = 100;
        m_layout = new Layout("root", Vertical, 1);
        m_layout->parent_ptr = nullptr;
        m_obj_ptrs.emplace("root", m_layout);
    }

    /**
     * @brief Get the singleton display instance
     *
     * @return Display*
     */
    Display *Display::get_display()
    {
        if (!m_instance)
            m_instance = new Display();
        return m_instance;
    }

    /**
     * @brief Setup the environment needed to run the display on different platforms
     *
     */
    void Display::init()
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
// define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
        // define something for Windows (64-bit only)
#else
        // define something for Windows (32-bit only)
#endif

#elif __APPLE__
#include <TargetConditionals.h>
#elif __linux__

#elif __unix__ // all unices not caught above

#elif defined(_POSIX_VERSION)
        // POSIX
#else
#error "Unknown compiler"
#endif

        // universal setup
        std::setlocale(LC_ALL, "en_US.UTF-8");
        std::cin.tie(NULL);
    }

    /**
     * @brief Update the size information of the terminal screen
     *
     * @return if display size has changed
     */
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

    /**
     * @brief Get the size of terminal screen
     *
     * @return Size of the terminal screen
     */
    Size Display::get_size()
    {
        update_size();
        return Size{m_width, m_height};
    }

    /**
     * @brief Get the height of the screen
     *
     * @return The height of the screen
     */
    int Display::get_height() const
    {
        return m_height;
    }

    /**
     * @brief Get the width of the screen
     *
     * @return The width of the screen
     */
    int Display::get_width() const
    {
        return m_width;
    }

    /**
     * @brief Set a pixel on the screen
     *
     * @param caller Ptr to the object calling this funciton (usually this)
     * @param x The x coord of the pixel to be set
     * @param y The y coord of the pixel to be set
     * @param px Pixel object to be placed at this position
     */
    void Display::set_pixel(BaseDisplayObject *caller, int x, int y, const Pixel &px)
    {
        if (x += caller->m_loc.x, y += caller->m_loc.y; x > -1 && x < m_width && y > -1 && y < m_height - 1)
        {
            m_screen[y][x] = px;
        }
    }

    void Display::set_pixel(BaseDisplayObject *caller, const Position &loc, const Pixel &px)
    {
        set_pixel(caller, loc.x, loc.y, px);
    }

    void Display::set_pixel(BaseDisplayObject *caller, const Position &loc, const Position &offset, const Pixel &px)
    {
        set_pixel(caller, loc + offset, px);
    }

    void Display::set_pixel(BaseDisplayObject *caller, int x, int y, wchar_t c, Style foreground, Style background, bool bold, bool underline, bool reversed)
    {
        if (x += caller->m_loc.x, y += caller->m_loc.y; x > -1 && x < m_width && y > -1 && y < m_height - 1)
        {
            m_screen[y][x].data = c;
            m_screen[y][x].foreground = foreground;
            m_screen[y][x].background = background;
            m_screen[y][x].bold = bold;
            m_screen[y][x].underline = underline;
            m_screen[y][x].reversed = reversed;
        }
    }

    bool Display::add_obj(std::string layout_name, std::string obj_name, BaseDisplayObject *o)
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
                LayoutObjects &parent_objs = *(static_cast<Layout *>(layout_pair_it->second)->get_objects());
                parent_objs.emplace_back(o);
                // sort the objects, separate fixed size and flexible size
                std::sort(parent_objs.begin(), parent_objs.end(), [](BaseDisplayObject *a, BaseDisplayObject *b)
                          { return a->get_size().fixed > b->get_size().fixed; });
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

    BaseDisplayObject *Display::get_obj(std::string name)
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
#if __APPLE__
            std::wcout << "\e[?1049h" << std::endl;
#elif __linux__
            std::wcout << "\e[?47h" << std::endl;
#endif
            // hide cursor
            std::wcout << "\e[?25l" << std::endl;
            // init refresh
            refreshLayout(m_layout, 0, 0, m_height, m_width, true);
            // create thread for display refresh
            m_refresh_thread = std::thread(&Display::refresh, this);
            // create thread for mouse input handle
            m_mouse_in_thread = std::thread(&Display::mouse_handler, this);
            // create thread for key press input
            m_key_in_thread = std::thread(&Display::key_handler, this);
        }
    }

    bool Display::has_power() const
    {
        return m_power;
    }

    void Display::power_off()
    {
        if (m_power)
        {
            m_power = false;
            // join display thread
            m_refresh_thread.join();
            // detach mouse thread and force terminate
            m_mouse_in_thread.detach();
            // detach keyboard thread and force terminate
            m_key_in_thread.detach();
            // enable cursor
            std::wcout << "\e[?25h" << std::endl;
            // leave alternate buffer
#if __APPLE__
            std::wcout << "\e[?1049l" << std::endl;
#elif __linux__
            std::wcout << "\e[?1047l" << std::endl;
#endif
            std::wcout << "Finished with exit code 0" << std::endl;
        }

        // if power off all is set to true. terminate all threads
        if (!m_power_off_all)
        {
            std::exit(0);
        }
    }

    inline void Display::clear_terminal()
    {
        std::wcout << "\x1B[2J\x1B[H";
    }

    void Display::clear_buffer()
    {
        for (auto &row : m_screen)
        {
            std::fill(row.begin(), row.end(), Pixel());
        }
    }

    // print out the content of the screen to the terminal
    inline void Display::output_screen()
    {
        // print every line except for last line
        for (int j = 0; j < m_height - 1; j++)
        {
            for (int i = 0; i < m_width; i++)
            {
                std::wcout << m_screen[j][i];
            }
            std::wcout << "\n";
        }
        // print last lines
        for (int i = 0; i < m_width; i++)
        {
            std::wcout << m_screen[m_height - 1][i];
        }
    }

    void Display::refresh()
    {
        while (m_power)
        {
            // get size update status
            bool is_resize = update_size();
            // update layout
            clear_buffer();
            refreshLayout(m_layout, 0, 0, m_height, m_width, is_resize);
            // output screen
            clear_terminal();
            output_screen();
            // wait for next refresh
            std::this_thread::sleep_for(std::chrono::milliseconds(m_refresh_interval));
        }
    }

    void Display::set_refresh_interval(int ms)
    {
        m_refresh_interval = ms;
    }

    void Display::refreshLayout(Layout *layout, int x, int y, int max_height, int max_width, bool is_resize)
    {

        LayoutObjects &objects = *(layout->get_objects());

        int sum_weight = std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, BaseDisplayObject *o)
                                         { return a + o->m_weight; });

        if (layout->orientation == Horizontal)
        {
            for (auto object : objects)
            {
                if (is_resize)
                {
                    object->m_size.height = max_height;
                    object->m_size.width = std::floor(1.0f * max_width * object->m_weight / sum_weight);
                }

                if (typeid(*object) == typeid(Layout))
                {
                    refreshLayout(static_cast<Layout *>(object), x, y, max_height, object->m_size.width, is_resize);
                }

                object->draw();

                object->m_loc.x = x;
                object->m_loc.y = y;
                x += object->m_size.width;
            }
        }

        if (layout->orientation == Vertical)
        {
            for (auto object : objects)
            {
                if (is_resize)
                {
                    object->m_size.width = max_width;
                    object->m_size.height = std::floor(1.0f * max_height * object->m_weight / sum_weight);
                }

                if (typeid(*object) == typeid(Layout))
                {
                    refreshLayout(static_cast<Layout *>(object), x, y, object->m_size.height, max_width, is_resize);
                }

                object->draw();

                object->m_loc.x = x;
                object->m_loc.y = y;
                y += object->m_size.height;
            }
        }
    }

    void Display::mouse_handler()
    {
        // enable special keywords
        system("stty -icanon");
        // disable echo
        system("stty -echo");

        while (m_power)
        {
            snprintf(mouse_data, 17, "\e[?1003h");
            // TODO: Process mouse data
            // for (int i = 0; i < 17; i++)
            //     std::wcout << std::hex << mouse_data[i];
        }

        // xterm disable mouse tracking
        system("echo \"\e[?1000l\"");
        // enable echo
        system("stty echo");
    }

    void Display::key_handler()
    {
        // read and store key press in display
        while (m_power)
        {
            m_key_press = std::getchar();
            invoke_key_action(m_key_press);
        }
    }

    bool Display::map_key_action(const char &c, std::function<void()> f)
    {
        if (m_action_map.find(c) != m_action_map.end())
        {
            return false;
        }
        else
        {
            m_action_map[c] = f;
        }
        return true;
    }

    bool Display::has_key_action(const char &c) const
    {
        if (m_action_map.find(c) != m_action_map.end())
        {
            return true;
        }
        return false;
    }

    bool Display::rm_key_action(const char &c)
    {
        if (m_action_map.find(c) != m_action_map.end())
        {
            m_action_map.erase(c);
            return true;
        }
        return false;
    }

    void Display::invoke_key_action(const char &c)
    {
        if (has_key_action(c))
        {
            m_action_map.at(c)();
        }
    }

    char Display::get_key_press() const
    {
        return m_key_press;
    }

    void Display::set_power_off_all(bool b)
    {
        m_power_off_all = b;
    }
}