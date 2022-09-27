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

    class BaseDisplayObject
    {
    public:
        BaseDisplayObject();

        int get_height() const;
        int get_width() const;
        int get_weight() const;
        Size get_size() const;
        Position get_loc() const;

        BaseDisplayObject *parent_ptr;
        std::string _name;

    protected:
        Position m_loc;
        Size m_size;
        int m_weight;
        Display *m_display_ptr;

        virtual void draw();
        friend class Display;
    };

    class Layout : public BaseDisplayObject
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

        // getters
        Size get_size();
        int get_width() const;
        int get_height() const;

        // painters
        void set_pixel(BaseDisplayObject *caller, int x, int y, const Pixel &px);
        void set_pixel(BaseDisplayObject *caller, const Position &loc, const Pixel &px);
        void set_pixel(BaseDisplayObject *caller, const Position &loc, const Position &offset, const Pixel &px);
        void set_pixel(BaseDisplayObject *caller, int x, int y, wchar_t c, Style foreground, Style background, bool bold = false, bool underline = false, bool reversed = false);

        // object management
        bool add_obj(std::string layout_name, std::string obj_name, BaseDisplayObject *o);
        bool remove_obj(std::string obj_name);
        BaseDisplayObject *get_obj(std::string obj_name);
        // display management
        void power_on();
        void power_off();
        void clear_buffer();
        void clear_terminal();
        bool update_size();
        void set_refresh_interval(int ms);
        void output_screen();
        void refreshLayout(Layout *layout, int x, int y, int max_height, int max_width, bool is_refresh);

        // input action management
        bool map_key_action(const char &c, std::function<void()> f);
        bool has_key_action(const char &c) const;
        bool rm_key_action(const char &c);
        void invoke_key_action(const char &c);

        // power properties
        void set_power_off_all(bool b);

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
        // display refresh utilities
        void refresh();
        std::thread m_refresh_thread;
        int m_refresh_interval;
        // user input handler
        char mouse_data[18];
        void mouse_handler();
        std::thread m_mouse_in_thread;
        // keyboard handler
        char key_data;
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