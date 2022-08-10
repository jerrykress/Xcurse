#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include <numeric>
#include <mutex>
#include <math.h>

#include "logger.h"

#define MAX_BUF_W 1000
#define MAX_BUF_H 1000

// forward declare
namespace Xcurse
{
    class Display;
    class GenericDisplayObject;
    struct ObjInfo;
}

namespace Xcurse
{
    typedef std::vector<std::vector<char>> Screen;
    typedef std::vector<GenericDisplayObject *> LayoutObjects;
    typedef std::map<std::string, GenericDisplayObject *> ObjTable;

    enum Direction
    {
        Horizontal,
        Vertical
    };

    class GenericDisplayObject
    {
    public:
        GenericDisplayObject *parent_ptr;

    protected:
        const std::string _name;
        int m_x, m_y, m_width, m_height, size_units;
        Display *m_display_ptr;
        Screen m_buffer;
        virtual void draw();
        virtual void resize(int w, int h);
        virtual void refresh_buffer();
        virtual void clear_buffer();
        friend class Display;
    };

    class Text : public GenericDisplayObject
    {
    public:
        Text(std::string data) : m_data(data)
        {
            m_height = 1;
            m_width = data.size();
        }
        void set_data(std::string data);
        std::string get_data() const;

    protected:
        std::string m_data;
    };

    class Window : public GenericDisplayObject
    {
    public:
        Window(std::string name, int size, std::string border);
        const std::string _name;

    protected:
        void refresh_buffer() override;
        void draw() override;
        std::string m_border;
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

        LayoutObjects &get_objects()
        {
            return m_objects;
        }

    protected:
        LayoutObjects m_objects;
        void draw() override{};
        void refresh_buffer() override;
        friend class Display;
    };

    struct Position
    {
        int x, y;
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
        static std::mutex screen_mutex;

        // getters
        Display::Size get_size();
        int get_width() const;
        int get_height() const;

        // painters
        void set_pixel(int x, int y, char c);

        // object management
        bool add_obj(std::string layout_name, std::string obj_name, GenericDisplayObject *o);
        bool remove_obj(std::string obj_name);
        // display management
        void power_on();
        void power_off();
        void clear_buffer();
        void clear_terminal();
        bool update_size();
        void set_refresh_interval(int ms);
        void output_screen();
        void refreshLayout(Layout *layout, int x, int y, int max_height, int max_width);

        void status() const;

    private:
        static Display *m_instance;
        // private constructor to prevent new instances
        Display();
        Display(const Display &);
        Display &operator=(const Display &);
        // main display properties
        Screen m_screen;
        bool m_power;
        int m_width, m_height;
        // display refresh utilities
        void refresh();
        std::thread m_refresh_thread;
        int m_refresh_interval;
        // windows properties
        Layout *m_layout;
        ObjTable m_obj_ptrs;
    };

}