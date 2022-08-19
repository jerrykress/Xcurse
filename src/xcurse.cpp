#include "xcurse.h"

using namespace Xcurse;
using namespace std::literals::chrono_literals;

// Initialise instance pointer
Display *Display::m_instance = NULL;

Display::Display()
{
    // setup basic attr
    update_size();
    m_screen = Screen(MAX_BUF_H, std::vector<Pixel>(MAX_BUF_W, Pixel{}));
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

/*
    Setup the environment needed to run the display on different platforms
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
#if TARGET_IPHONE_SIMULATOR
    // iOS, tvOS, or watchOS Simulator
#elif TARGET_OS_MACCATALYST
    // Mac's Catalyst (ports iOS API into Mac, like UIKit).
#elif TARGET_OS_MAC
    // Other kinds of Apple platforms
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
    // on linux, setup the locale to output unicode characters
    std::locale::global(std::locale("en_US.utf8"));
    std::wcout.imbue(std::locale());
#elif __unix__ // all unices not caught above
    // Unix
    // on linux, setup the locale to output unicode characters
    std::locale::global(std::locale("en_US.utf8"));
    std::wcout.imbue(std::locale());
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#error "Unknown compiler"
#endif

    // on windows, setup the wide char mode
    // _setmode(_fileno(stdout), _O_WTEXT);
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

Size Display::get_size()
{
    update_size();
    return Size{m_width, m_height};
}

int Display::get_height() const
{
    return m_height;
}

int Display::get_width() const
{
    return m_width;
}

void Display::set_pixel(GenericDisplayObject *caller, int x, int y, const Pixel &px)
{
    if (x += caller->m_loc.x, y += caller->m_loc.y; x > -1 && x < m_width && y > -1 && y < m_height - 1)
    {
        m_screen[y][x] = px;
    }
}

void Display::set_pixel(GenericDisplayObject *caller, const Position &loc, const Pixel &px)
{
    set_pixel(caller, loc.x, loc.y, px);
}

void Display::set_pixel(GenericDisplayObject *caller, const Position &loc, const Position &offset, const Pixel &px)
{
    set_pixel(caller, loc + offset, px);
}

void Display::set_pixel(GenericDisplayObject *caller, int x, int y, wchar_t c, Style foreground, Style background, bool bold, bool underline, bool reversed)
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
        // init refresh
        refreshLayout(m_layout, 0, 0, m_height, m_width, true);
        // create thread for display refresh
        m_refresh_thread = std::thread(&Display::refresh, this);
        // create thread for mouse input handle
        m_mouse_in_thread = std::thread(&Display::mouse_handler, this);
    }
}

void Display::power_off()
{
    if (m_power)
    {
        m_power = false;
        // join display thread
        m_refresh_thread.join();
        // join mouse thread
        m_mouse_in_thread.join();
        // enable cursor
        std::cout << "\e[?25h" << std::endl;
        // leave alternate buffer
        std::cout << "\e[?1047l" << std::endl;
        // TODO: fix getchar issue to remove this line
        std::cout << "Press any key to exit..." << std::endl;
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
        std::fill(row.begin(), row.end(), Pixel{});
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
        clear_terminal();
        clear_buffer();
        // get size update status
        bool is_resize = update_size();
        // update layout
        refreshLayout(m_layout, 0, 0, m_height, m_width, is_resize);
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

void Display::refreshLayout(Layout *layout, int x, int y, int max_height, int max_width, bool is_resize)
{

    LayoutObjects &objects = *(layout->get_objects());

    int sum_weight = std::accumulate(objects.begin(), objects.end(), 0, [&layout](int a, GenericDisplayObject *o)
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
        //     std::cout << std::hex << mouse_data[i];

        key_data = getchar();
    }

    // xterm disable mouse tracking
    system("echo \"\e[?1000l\"");
    // enable echo
    system("stty echo");
}
