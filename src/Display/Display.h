#pragma once

#include <map>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <numeric>
#include <math.h>
#include <atomic>
#include <algorithm>

#include "../Common/Common.h"

namespace Xcurse
{
    /**
     * @brief The base template of all display objects
     * @note Implement draw() to interface with the display
     *
     */
    class BaseDisplayObject
    {
    public:
        BaseDisplayObject();

        virtual int get_height() const;
        virtual int get_width() const;
        virtual int get_weight() const;
        virtual Size get_size() const;
        Position get_loc() const;
        void set_visible();
        void set_visible(bool b);

        BaseDisplayObject *parent_ptr;

    protected:
        Position m_loc;
        Size m_size;
        int m_weight;
        std::atomic_bool m_visible;
        Display *m_display_ptr;

        virtual void draw();
        friend class Display;
    };

    /**
     * @brief A structure used organise objects, contains layout and other objects
     *
     */
    class Layout : public BaseDisplayObject
    {
    public:
        Layout(Direction direction, int size);
        Direction orientation;

        LayoutObjects *get_objects();

    protected:
        LayoutObjects m_objects;
        void draw() override{};
        friend class Display;
    };

    /**
     * @brief The main active component that contains a single instance of running display. Handles all rendering and I/O process
     *
     */
    class Display
    {

    public:
        static Display *get_display();
        static void init();

        // getters
        Size get_size();
        int get_width() const;
        int get_height() const;

        // pixel setter
        void set_pixel(BaseDisplayObject *caller, int x, int y, const Pixel &px);
        void set_pixel(BaseDisplayObject *caller, const Position &loc, const Pixel &px);
        void set_pixel(BaseDisplayObject *caller, const Position &loc, const Position &offset, const Pixel &px);
        void set_pixel(BaseDisplayObject *caller, int x, int y, const wchar_t &c, const Style &foreground, const Style &background, bool bold = false, bool underline = false, bool reversed = false);
        void set_pixels(BaseDisplayObject *caller, int x, int y, const std::wstring &ws, Stylable s = Stylable());

        // object management
        bool add_obj(std::string layout_name, std::string obj_name, BaseDisplayObject *o);
        bool remove_obj(std::string obj_name);
        BaseDisplayObject *get_obj(std::string obj_name);

        // power settings
        void power_on();
        bool has_power() const;
        void power_off();
        void set_power_off_all(bool b);

        // cursor tools
        void reset_cursor();
        void set_cursor_x(int x);
        void set_cursor_y(int y);
        void set_cursor_pos(Position p);

        // refresh tools
        void clear_buffer();
        void clear_terminal();
        bool update_size();
        void set_refresh_interval(int ms);
        void output_screen();
        void refreshLayout(Layout *layout, int x, int y, int max_height, int max_width, bool is_refresh);
        void update_layout();

        // alt screen setting
        void set_min_screen_size(Size s);
        void set_alt_screen(BaseDisplayObject *o);
        void enable_alt_screen(bool b);
        void output_alt_screen();

        // input action management
        void set_io_mouse(bool b);
        void set_io_kb(bool b);
        bool map_key_action(const char &c, std::function<void()> f);
        bool has_key_action(const char &c) const;
        bool rm_key_action(const char &c);
        void invoke_key_action(const char &c);
        char get_key_press() const;

        BaseDisplayObject *&operator[](std::string name);

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
        Size m_min_screen_size;
        BaseDisplayObject *m_alt_screen;
        bool m_enable_alt;

        // display refresh utilities
        void refresh();
        std::thread m_refresh_thread;
        int m_refresh_interval;
        std::atomic_bool m_update_layout;

        // mouse input handler
        bool m_mouse_status;
        char mouse_data[18];
        void mouse_handler();
        std::thread m_mouse_in_thread;

        // keyboard handler
        bool m_kb_status;
        char m_key_press;
        void key_handler();
        std::thread m_key_in_thread;

        // input action map
        std::unordered_map<char, std::function<void()>> m_action_map;

        // windows properties
        Layout *m_layout;
        ObjTable m_obj_ptrs;

        // power properties
        bool m_power_off_all;
    };

}