#include "Display.h"

namespace Xcurse
{
    using namespace std::literals::chrono_literals;

    // Initialise instance pointer
    Display *Display::m_instance = NULL;

    /**
     * @brief Get the pointer to a BaseDisplayObject from the current display
     * @note Manual casting to the desired object type is required
     * @param name
     * @return BaseDisplayObject*&
     */
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
        // get init screen size
        update_size();
        // alloc buffer
        m_screen = Screen(MAX_BUF_H, std::vector<Pixel>(MAX_BUF_W, Pixel()));
        // default power off
        m_power = false;
        // default enable power off all threads
        m_power_off_all = true;
        // don't update layout immediately upon power on
        m_update_layout = false;
        // default min screen size is 0,0
        m_min_screen_size = Size{0, 0};
        // default refresh interval is 100ms
        m_refresh_interval = 100;
        // insert root layout with no parent
        m_layout = new Layout("root", Vertical, 1);
        m_layout->parent_ptr = nullptr;
        m_obj_ptrs.emplace("root", m_layout);
        // default key press to space
        m_key_press = ' ';
        // default no alternate screen
        m_alt_screen = nullptr;
        // default mouse status
        m_mouse_status = false;
        // default keyboard status
        m_kb_status = true;
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
     * @brief Set the minimum screen size required for the program to function. An alternative screen is shown if current screen size is smaller than the set minimum size.
     *
     * @param s
     */
    void Display::set_min_screen_size(Size s)
    {
        m_min_screen_size = s;
    }

    /**
     * @brief Get the size of terminal screen
     *
     * @return Size of the terminal screen
     */
    inline Size Display::get_size()
    {
        update_size();
        return Size{m_width, m_height};
    }

    /**
     * @brief Get the height of the screen
     *
     * @return The height of the screen
     */
    inline int Display::get_height() const
    {
        return m_height;
    }

    /**
     * @brief Get the width of the screen
     *
     * @return The width of the screen
     */
    inline int Display::get_width() const
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
        if (x += caller->m_loc.x, y += caller->m_loc.y; x > -1 && x < m_width && y > -1 && y < m_height)
        {
            m_screen[y][x] = px;
        }
    }

    /**
     * @brief Set a pixel on the screen
     *
     * @param caller Ptr to the object calling this funciton (usually this)
     * @param loc A Position object of the pixel to be set
     * @param px Pixel object to be placed at this position
     */
    void Display::set_pixel(BaseDisplayObject *caller, const Position &loc, const Pixel &px)
    {
        set_pixel(caller, loc.x, loc.y, px);
    }

    /**
     * @brief Set a pixel on the screen
     *
     * @param caller Ptr to the object calling this funciton (usually this)
     * @param loc A Position object of the pixel to be set
     * @param offset An offset position applied on top of loc
     * @param px Pixel object to be placed at this position
     */
    void Display::set_pixel(BaseDisplayObject *caller, const Position &loc, const Position &offset, const Pixel &px)
    {
        set_pixel(caller, loc + offset, px);
    }

    /**
     * @brief Set a pixel on the screen
     *
     * @param caller A Position object of the pixel to be set
     * @param x X coord
     * @param y y coord
     * @param c Pixel character
     * @param foreground foreground color
     * @param background background color
     * @param bold enable bold
     * @param underline enable underline
     * @param reversed enable reversed
     */
    void Display::set_pixel(BaseDisplayObject *caller, int x, int y, wchar_t c, Style foreground, Style background, bool bold, bool underline, bool reversed)
    {
        if (x += caller->m_loc.x, y += caller->m_loc.y; x > -1 && x < m_width && y > -1 && y < m_height)
        {
            m_screen[y][x].data = c;
            m_screen[y][x].foreground = foreground;
            m_screen[y][x].background = background;
            m_screen[y][x].bold = bold;
            m_screen[y][x].underline = underline;
            m_screen[y][x].reversed = reversed;
        }
    }

    /**
     * @brief Add a Display Object to a layout
     *
     * @param layout_name Target layout
     * @param obj_name Object name
     * @param o Object to be added
     *
     * @return If insertion was successful
     */
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
                // link to current display
                o->m_display_ptr = get_display();
                // add object pointer to parent's record
                LayoutObjects &parent_objs = *(static_cast<Layout *>(layout_pair_it->second)->get_objects());
                parent_objs.emplace_back(o);
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Remove an object from a layout
     *
     * @param obj_name Object to be removed
     *
     * @return If object was successful removed
     */
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

    /**
     * @brief Get the pointer to an existing display object
     *
     * @param name Name of object
     * @return Pointer to the retrieved object
     */
    BaseDisplayObject *Display::get_obj(std::string name)
    {
        if (m_obj_ptrs.count(name))
        {
            return m_obj_ptrs[name];
        }
        return nullptr;
    }

    /**
     * @brief Power on the display. Also start all threads handling display output, keyboard and mouse input
     *
     */
    void Display::power_on()
    {
        if (!m_power)
        {
            m_power = true;
            // enter alternate buffer
#if __APPLE__
            std::wcout << "\e[?1049h" << std::endl;
#elif __linux__
            std::wcout << "\e[?1049h" << std::endl;
#endif
            // hide cursor
            std::wcout << "\e[?25l" << std::endl;
            // enable special keywords
            system("stty -icanon");
            // disable echo
            system("stty -echo");
            // init refresh
            refreshLayout(m_layout, 0, 0, m_height, m_width, true);
            // create thread for display refresh
            m_refresh_thread = std::thread(&Display::refresh, this);
            // create thread for key press input
            m_key_in_thread = std::thread(&Display::key_handler, this);
            // create thread for mouse input handle
            m_mouse_in_thread = std::thread(&Display::mouse_handler, this);
        }
    }

    /**
     * @brief If the display is powered on
     *
     * @return true
     * @return false
     */
    bool Display::has_power() const
    {
        return m_power;
    }

    /**
     * @brief Power off the display. Also terminates all threads handling display output, keyboard and mouse input
     *
     */
    void Display::power_off()
    {
        if (m_power)
        {
            m_power = false;
            // xterm disable mouse tracking
            system("echo \"\e[?1000l\"");
            // enable echo
            system("stty echo");
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
            std::wcout << "\e[?1049l" << std::endl;
#endif
            std::wcout << "Finished with exit code 0" << std::endl;
        }

        // if power off all is set to true. terminate all threads
        if (!m_power_off_all)
        {
            std::exit(0);
        }
    }

    /**
     * @brief Clear the terminal screen
     *
     */
    inline void Display::clear_terminal()
    {
        std::wcout << "\x1B[2J\x1B[H";
    }

    /**
     * @brief ANSI Code - Move cursor to upper left corner
     *
     */
    inline void Display::reset_cursor()
    {
        std::wcout << "\e[H";
    }

    /**
     * @brief ANSI Code - Move cursor to X position
     *
     * @param x
     */
    inline void Display::set_cursor_x(int x)
    {
        std::wcout << "\e[" << std::to_wstring(x) << "G";
    }

    /**
     * @brief ANSI Code - Move cursor to Y position
     *
     * @param y
     */
    inline void Display::set_cursor_y(int y)
    {
        std::wcout << "\e[" << std::to_wstring(y) << "B";
    }

    /**
     * @brief ANSI Code - Move cursor to x,y position
     *
     * @param p
     */
    inline void Display::set_cursor_pos(Position p)
    {
        std::wcout << "\e[{" << std::to_wstring(p.y) << "};{" << std::to_wstring(p.x) << "}H";
        std::wcout << "\e[{" << std::to_wstring(p.y) << "};{" << std::to_wstring(p.x) << "}f";
    }

    /**
     * @brief Clear the display buffer
     *
     */
    void Display::clear_buffer()
    {
        for (auto &row : m_screen)
        {
            std::fill(row.begin(), row.end(), Pixel());
        }
    }

    /**
     * @brief Output the content of the display buffer onto terminal screen
     *
     */
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

    /**
     * @brief Set alt screen to be displayed when screen size is too small
     *
     * @param o
     */
    void Display::set_alt_screen(BaseDisplayObject *o)
    {
        m_alt_screen = o;
        m_alt_screen->m_display_ptr = this;
    }

    /**
     * @brief Enable alt screen
     *
     * @param b
     */
    void Display::enable_alt_screen(bool b)
    {
        m_enable_alt = b;
    }

    /**
     * @brief Output the alt screen instead of the main screen
     *
     */
    inline void Display::output_alt_screen()
    {
        if (m_alt_screen != nullptr)
        {
            m_alt_screen->draw();
        }
        else
        {
            clear_terminal();
            std::wcout << L"Screen is too small\n"
                       << L"\nCurrent size: " << get_size()
                       << L"Requires size: " << m_min_screen_size
                       << std::endl;
        }
    }

    /**
     * @brief Update the layouts and objects on the display
     *
     */
    void Display::refresh()
    {
        while (m_power)
        {
            // resize if size is updated or explictly told to
            bool is_resize = update_size() || m_update_layout;
            // if screen is too small and alt is enabled, print alt screen
            if (m_enable_alt && get_size() < m_min_screen_size)
            {
                output_alt_screen();
            }
            else
            {
                // clear screen buffer and update layout from root
                clear_buffer();
                refreshLayout(m_layout, 0, 0, m_height, m_width, is_resize);
                // set cursor to 0,0 and output screen
                reset_cursor();
                output_screen();
            }
            // wait for next refresh
            std::this_thread::sleep_for(std::chrono::milliseconds(m_refresh_interval));
        }
    }

    /**
     * @brief Set how often the display updates
     *
     * @param ms
     */
    void Display::set_refresh_interval(int ms)
    {
        m_refresh_interval = ms;
    }

    /**
     * @brief Recursively update the size of the layouts
     *
     * @param layout Layout to be updated
     * @param x X coord of layout upper left corner
     * @param y Y coord of layout upper left corner
     * @param max_height Max available height
     * @param max_width Max available width
     * @param is_resize Whether to update the layout size
     */
    void Display::refreshLayout(Layout *layout, int x, int y, int max_height, int max_width, bool is_resize)
    {

        LayoutObjects &objects = *(layout->get_objects());

        int sum_weight = std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, BaseDisplayObject *o)
                                         { return a + (o->m_visible && !o->m_size.fixed) * o->m_weight; });

        if (layout->orientation == Horizontal)
        {
            max_width -= std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, BaseDisplayObject *o)
                                         { return a + (o->m_visible && o->m_size.fixed) * o->m_size.width; });

            for (auto object : objects)
            {
                if (object->m_visible)
                {
                    if (is_resize)
                    {
                        object->m_size.height = max_height;
                        if (!object->m_size.fixed)
                        {
                            object->m_size.width = std::floor(1.0f * max_width * object->m_weight / sum_weight);
                        }
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
        }

        // if (layout->orientation == Vertical)
        else
        {
            max_height -= std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, BaseDisplayObject *o)
                                          { return a + (o->m_visible && o->m_size.fixed) * o->m_size.height; });

            for (auto object : objects)
            {
                if (object->m_visible)
                {
                    if (is_resize)
                    {
                        object->m_size.width = max_width;
                        if (!object->m_size.fixed)
                        {
                            object->m_size.height = std::floor(1.0f * max_height * object->m_weight / sum_weight);
                        }
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
    }

    /**
     * @brief Explicitly tell display to update layouts of all visible objects
     *
     */
    void Display::update_layout()
    {
        m_update_layout = true;
    }

    /**
     * @brief Spawn a thread that reads and processes mouse input
     *
     */
    void Display::mouse_handler()
    {
        if (m_mouse_status)
        {
            while (m_power)
            {
                snprintf(mouse_data, 17, "\e[?1003h");
                // TODO: Process mouse data
                // for (int i = 0; i < 17; i++)
                //     std::wcout << std::hex << mouse_data[i];
            }
        }
    }

    /**
     * @brief Spawn a thread that reads and handles keyboard input
     *
     */
    void Display::key_handler()
    {
        if (m_kb_status)
        {
            // read and store key press in display
            while (m_power)
            {
                m_key_press = std::getchar();
                invoke_key_action(m_key_press);
            }
        }
    }

    /**
     * @brief Map a keypress to a function
     *
     * @param c Char of keypress
     * @param f Lambda function to be invoked
     *
     * @return If insertion was successful
     */
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

    /**
     * @brief Find if a key is mapped to an action
     *
     * @param c Char of keypress
     *
     * @return If insertion was successful
     */
    bool Display::has_key_action(const char &c) const
    {
        if (m_action_map.find(c) != m_action_map.end())
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Remove an action from a keybind
     *
     * @param c Char of keypress
     *
     * @return If deletion was successful
     */
    bool Display::rm_key_action(const char &c)
    {
        if (m_action_map.find(c) != m_action_map.end())
        {
            m_action_map.erase(c);
            return true;
        }
        return false;
    }

    /**
     * @brief Invoke an action mapped to a keypress
     *
     * @param c Char of keypress
     */
    void Display::invoke_key_action(const char &c)
    {
        if (has_key_action(c))
        {
            m_action_map.at(c)();
        }
    }

    /**
     * @brief Get the last registered keypress
     *
     * @return char
     */
    char Display::get_key_press() const
    {
        return m_key_press;
    }

    /**
     * @brief Set if all threads should be powered off when power off is called
     *
     * @param b
     */
    void Display::set_power_off_all(bool b)
    {
        m_power_off_all = b;
    }

    /**
     * @brief Set whether to process mouse input
     *
     * @param b
     */
    void Display::set_io_mouse(bool b)
    {
        m_mouse_status = b;
    }

    /**
     * @brief Set whether to process keyboard input
     *
     * @param b
     */
    void Display::set_io_kb(bool b)
    {
        m_kb_status = b;
    }
}