#include <vector>
#include <map>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <algorithm>
#include <numeric>
#include <mutex>
#include <math.h>
#include <atomic>
#include <locale>
#include <initializer_list>

#include "logger.h"

#define MAX_BUF_W 1000
#define MAX_BUF_H 1000

#define DEFAULT_WIN_BORDER L"--||++++"

#define TEXT_COLOR_RED L"\x1b[31m"
#define TEXT_COLOR_GREEN L"\x1b[32m"
#define TEXT_COLOR_YELLOW L"\x1b[33m"
#define TEXT_COLOR_BLUE L"\x1b[34m"
#define TEXT_COLOR_MAGENTA L"\x1b[35m"
#define TEXT_COLOR_CYAN L"\x1b[36m"
#define TEXT_COLOR_RESET L"\x1b[0m"

#define TEXT_COLOR_BRIGHT_BLACK L"\x1b[30;1m"
#define TEXT_COLOR_BRIGHT_RED L"\x1b[31;1m"
#define TEXT_COLOR_BRIGHT_GREEN L"\x1b[32;1m"
#define TEXT_COLOR_BRIGHT_YELLOW L"\x1b[33;1m"
#define TEXT_COLOR_BRIGHT_BLUE L"\x1b[34;1m"
#define TEXT_COLOR_BRIGHT_MAGENTA L"\x1b[35;1m"
#define TEXT_COLOR_BRIGHT_CYAN L"\x1b[36;1m"
#define TEXT_COLOR_BRIGHT_WHITE L"\x1b[37;1m"

#define BACKGROUND_COLOR_BLACK L"\x1b[40m"
#define BACKGROUND_COLOR_RED L"\x1b[41m"
#define BACKGROUND_COLOR_GREEN L"\x1b[42m"
#define BACKGROUND_COLOR_YELLOW L"\x1b[43m"
#define BACKGROUND_COLOR_BLUE L"\x1b[44m"
#define BACKGROUND_COLOR_MAGENTA L"\x1b[45m"
#define BACKGROUND_COLOR_CYAN L"\x1b[46m"
#define BACKGROUND_COLOR_WHITE L"\x1b[47m"
#define BACKGROUND_COLOR_RESET L"\x1b[0m"

#define BACKGROUND_COLOR_BRIGHT_BLACK L"\x1b[40;1m"
#define BACKGROUND_COLOR_BRIGHT_RED L"\x1b[41;1m"
#define BACKGROUND_COLOR_BRIGHT_GREEB L"\x1b[42;1m"
#define BACKGROUND_COLOR_BRIGHT_YELLOW L"\x1b[43;1m"
#define BACKGROUND_COLOR_BRIGHT_BLUE L"\x1b[44;1m"
#define BACKGROUND_COLOR_BRIGHT_MAGENTA L"\x1b[45;1m"
#define BACKGROUND_COLOR_BRIGHT_CYAN L"\x1b[46;1m"
#define BACKGROUND_COLOR_BRIGHT_WHITE L"\x1b[47;1m"

// forward declare
namespace Xcurse
{
    class Display;
    class GenericDisplayObject;
    struct ObjInfo;
    struct Pixel;
}

namespace Xcurse
{
    typedef std::vector<std::vector<Pixel>> Screen;
    typedef std::vector<GenericDisplayObject *> LayoutObjects;
    typedef std::map<std::string, GenericDisplayObject *> ObjTable;
    typedef std::wstring TextColor;
    typedef std::wstring BgColor;

    enum Direction
    {
        Horizontal,
        Vertical
    };

    struct Pixel
    {
        wchar_t data;
        TextColor tx_color;
        BgColor bg_color;
    };

    struct Position
    {
        int x, y;
    };

    class GenericDisplayObject
    {
    public:
        GenericDisplayObject *parent_ptr;
        virtual void clear_buffer();
        virtual void resize(int w, int h);
        int get_height() const;
        int get_width() const;
        int get_size() const;
        Position get_pos() const;

    protected:
        const std::string _name;
        int m_x, m_y, m_width, m_height, size_units;
        Display *m_display_ptr;
        Screen m_buffer;
        virtual void draw();
        virtual void refresh_buffer();
        friend class Display;
    };

    class Text : public GenericDisplayObject
    {
    public:
        Text(std::string data);
        void set_data(std::string data);
        std::string get_data() const;

    protected:
        std::string m_data;
    };

    /*
    Window Class
    */
    class Window : public GenericDisplayObject
    {
    public:
        Window(std::string name, int size, std::wstring border = DEFAULT_WIN_BORDER);
        const std::string _name;
        void add_char(int x, int y, wchar_t c, TextColor tx_color = TEXT_COLOR_RESET, BgColor bg_color = BACKGROUND_COLOR_RESET);
        void add_chars(const std::initializer_list<std::tuple<int, int, wchar_t, TextColor, BgColor>> &chars);
        void add_str(int x, int y, const std::wstring &w_str, TextColor color = TEXT_COLOR_RESET);

    protected:
        void refresh_buffer() override;
        void draw() override;
        std::wstring m_border;
    };

    /*
    Layout Class
    */
    class Layout : public GenericDisplayObject
    {
    public:
        Layout(std::string name, Direction direction, int size);
        const std::string _name;
        Direction orientation;

        LayoutObjects *get_objects();

    protected:
        LayoutObjects m_objects;
        void draw() override{};
        friend class Display;
    };

    class Display
    {
        struct Size
        {
            bool operator==(Size &that)
            {
                return width == that.width && height == that.height;
            }
            int width, height;
        };

    public:
        static Display *get_display();
        static void init();
        static std::mutex screen_mutex;

        // getters
        Display::Size get_size();
        int get_width() const;
        int get_height() const;

        // painters
        void set_pixel(int x, int y, wchar_t c, TextColor tx_color, BgColor bg_color);
        void set_pixel(int x, int y, Pixel px);

        // object management
        bool add_obj(std::string layout_name, std::string obj_name, GenericDisplayObject *o);
        bool remove_obj(std::string obj_name);
        GenericDisplayObject *get_obj(std::string obj_name);
        // display management
        void power_on();
        void power_off();
        void clear_buffer();
        void clear_terminal();
        bool update_size();
        void set_refresh_interval(int ms);
        void output_screen();
        void refreshLayout(Layout *layout, int x, int y, int max_height, int max_width, bool is_refresh);

        void status() const;

    private:
        static Display *m_instance;
        // private constructor to prevent new instances
        Display();
        Display(const Display &);
        Display &operator=(const Display &);
        // main display properties
        Screen m_screen;
        std::atomic_bool m_power;
        int m_width, m_height;
        // display refresh utilities
        void refresh();
        std::thread m_refresh_thread;
        int m_refresh_interval;
        // user input handler
        char mouse_data[18];
        void mouse_handler();
        std::thread m_mouse_in_thread;
        // keyboard handler
        int key_data;
        // windows properties
        Layout *m_layout;
        ObjTable m_obj_ptrs;
    };

}