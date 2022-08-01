#include <vector>
#include <unordered_map>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

// forward declare
namespace Xcurse
{
    class Display;
    class GenericWindowObject;
}

namespace Xcurse
{
    typedef std::vector<std::vector<char>> Screen;

    struct Position
    {
        int x, y;
    };

    class GenericWindowObject
    {
    public:
        // getters
        virtual int get_width() const { return m_width; }
        virtual int get_height() const { return m_height; }
        virtual int get_x() const { return m_x; }
        virtual int get_y() const { return m_y; }
        virtual std::string get_name() const { return m_name; }

        // setters
        virtual void set_width(int w) { m_width = w; }
        virtual void set_height(int h) { m_height = h; }
        virtual void set_x(int x) { m_x = x; }
        virtual void set_y(int y) { m_y = y; }

    protected:
        // attributes
        std::string m_name;
        int m_width, m_height;
        int m_x, m_y;
        std::string m_border;
        // draw window
        virtual void draw(Display *d) = 0;
        friend class Display;
    };

    class StaticWindow : public GenericWindowObject
    {
    public:
        explicit StaticWindow(std::string name, int x, int y, int width, int height, std::string border);

    private:
        void draw(Display *d);
        friend class Display;
    };

    class FlexibleWindow : public GenericWindowObject
    {
    public:
        explicit FlexibleWindow(std::string name, int x, int y, int w_percent, int h_percent, std::string border);
        // special getters
        int get_w_percent() const;
        int get_h_percent() const;
        // special setters
        void set_w_percent(int wp);
        void set_h_percent(int hp);

    private:
        int m_w_percent, m_h_percent;
        void draw(Display *d);
        friend class Display;
    };

    class Display
    {

    public:
        static Display *get_display();

        // getters
        std::pair<int, int> get_size();
        int get_width() const;
        int get_height() const;

        // painters
        void set_pixel(int x, int y, char c);

        // window management
        bool add_win(StaticWindow &w);
        bool add_win(FlexibleWindow &w);
        bool remove_win(std::string win_name);

        // display management
        void clear();
        void refresh();
        void set_refresh_interval(float t);

    private:
        static Display *m_instance;
        // private constructor to prevent new instances
        Display();
        Display(const Display &);
        Display &operator=(const Display &);
        // main display properties
        Screen m_screen;
        int m_width, m_height;
        // windows properties
        std::vector<StaticWindow> m_windows;
        std::unordered_map<std::string, std::vector<StaticWindow>::iterator> m_window_iterators;
    };

}