#include <vector>
#include <unordered_map>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include <numeric>

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
    typedef std::unordered_map<std::string, ObjInfo> ObjPtrMap;

    struct ObjInfo
    {
        GenericDisplayObject *obj_ptr, *parent_ptr;
    };

    enum Direction
    {
        Horizontal,
        Vertical
    };

    class GenericDisplayObject
    {
    public:
    protected:
        int m_x, m_y, m_width, m_height, size_units;
        Display *m_display_ptr;
        virtual void draw(){};
        virtual void resize(int w, int h){};

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
        void resize(int w, int h){};
        void draw(){};
    };

    class Window : public GenericDisplayObject
    {
    protected:
        Screen m_buffer;
    };

    class Layout : public GenericDisplayObject
    {
    public:
        Direction orientation;

        LayoutObjects &get_objects()
        {
            return m_objects;
        }

    protected:
        LayoutObjects m_objects;
        void draw(){};
        void resize(int w, int h){};
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
        void start();
        void poweroff();
        void clear();
        bool update_size();
        void refresh();
        void set_refresh_interval(int ms);
        void refresh_screen();
        void refresh_buffer();
        void refreshLayout(Layout &layout, int x, int y, int max_height, int max_width);

    private:
        static Display *m_instance;
        // private constructor to prevent new instances
        Display();
        Display(const Display &);
        Display &operator=(const Display &);
        // main display properties
        bool m_power;
        Screen m_screen;
        int m_width, m_height;
        // display refresh utilities
        std::thread m_display_thread;
        int m_refresh_interval_ms;
        // windows properties
        Layout m_layout;
        ObjPtrMap m_obj_ptrs;
    };

}