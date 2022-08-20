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

#include "color.h"
#include "logger.h"

#define MAX_BUF_W 500
#define MAX_BUF_H 500

#define DEFAULT_WIN_BORDER L"=-||++++"

// forward declare
namespace Xcurse
{
    class Display;
    class GenericDisplayObject;
    class Stylable;
    class Pixel;
}

namespace Xcurse
{
    typedef std::vector<std::vector<Pixel>> Screen;
    typedef std::vector<GenericDisplayObject *> LayoutObjects;
    typedef std::map<std::string, GenericDisplayObject *> ObjTable;
    typedef std::wstring Style;

    enum Direction
    {
        Horizontal,
        Vertical
    };

    class Stylable
    {
    public:
        Stylable();
        Stylable(Style foreground, Style background, bool bold, bool underline, bool reversed);
        Stylable(const Stylable &that);
        Stylable(Stylable &&that);
        Stylable &operator=(const Stylable &that) noexcept;
        Stylable &operator=(Stylable &&that) noexcept;
        ~Stylable();

        std::wstring style() const;

        Style foreground = TEXT_COLOR_RESET, background = BACKGROUND_COLOR_RESET;
        bool bold = false, underline = false, reversed = false;
    };

    class Pixel : public Stylable
    {
    public:
        Pixel();
        Pixel(wchar_t c);
        Pixel(wchar_t c, Stylable &s);
        Pixel(wchar_t c, Stylable &&s);
        Pixel(const Pixel &that);
        Pixel(Pixel &&that);
        Pixel &operator=(const Pixel &that) noexcept;
        Pixel &operator=(Pixel &&that) noexcept;
        ~Pixel();

        friend std::wostream &operator<<(std::wostream &out, const Pixel &px);

        wchar_t data = L' ';
    };

    struct Position
    {
        bool operator<(const Position &that) const;
        bool operator==(const Position &that) const;
        Position &operator+=(const Position &that);
        friend Position operator+(Position p1, const Position &p2)
        {
            p1 += p2;
            return p1;
        }

        int x = 0, y = 0;
    };

    struct Size
    {
        bool operator==(Size &that);

        int width = 0, height = 0;
    };

    class GenericDisplayObject
    {
    public:
        GenericDisplayObject();

        int get_height() const;
        int get_width() const;
        int get_weight() const;
        Size get_size() const;
        Position get_loc() const;

        GenericDisplayObject *parent_ptr;
        std::string _name;

    protected:
        Position m_loc;
        Size m_size;
        int m_weight;
        Display *m_display_ptr;

        virtual void draw();
        friend class Display;
    };

    class GenericTextObject : public GenericDisplayObject, public Stylable
    {
    public:
        void set_data(std::wstring data);
        std::wstring get_data() const;

    protected:
        std::wstring m_data;
    };

    /*
    Window Class
    */
    class GenericWindowObject : public GenericDisplayObject, public Stylable
    {
    public:
        GenericWindowObject();
        void set_title(std::wstring title);
        void set_show_border(bool b);
        void set_show_titlebar(bool b);
        void set_override_win_style(bool b);

    protected:
        void draw_border();
        void draw_titlebar();

        std::wstring m_title = L" Title ";
        std::wstring m_border = DEFAULT_WIN_BORDER;
        bool override_win_style = false, show_border = true, show_titlebar = true;
    };

    class GridWindow : public GenericWindowObject
    {
        typedef std::map<Position, Pixel> WindowData;

    public:
        GridWindow(std::string name, int size, std::wstring border = DEFAULT_WIN_BORDER);
        void add_char(int x, int y, wchar_t c, Style foreground = TEXT_COLOR_RESET, Style background = BACKGROUND_COLOR_RESET, bool bold = false, bool underline = false, bool reversed = false);
        bool clear_char(int x, int y);
        void clean();

    protected:
        WindowData m_windata;
        void draw() override;
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

    public:
        static Display *get_display();
        static void init();
        static std::mutex screen_mutex;

        // getters
        Size get_size();
        int get_width() const;
        int get_height() const;

        // painters
        void set_pixel(GenericDisplayObject *caller, int x, int y, const Pixel &px);
        void set_pixel(GenericDisplayObject *caller, const Position &loc, const Pixel &px);
        void set_pixel(GenericDisplayObject *caller, const Position &loc, const Position &offset, const Pixel &px);
        void set_pixel(GenericDisplayObject *caller, int x, int y, wchar_t c, Style foreground, Style background, bool bold = false, bool underline = false, bool reversed = false);

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

        GenericDisplayObject *&operator[](std::string name);

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