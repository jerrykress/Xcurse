#include <vector>
#include <map>
#include <string>
#include "constants.h"

namespace Xcurse
{
    class Display;
    class GenericDisplayObject;
    class Stylable;
    class Pixel;

    /**
     * @brief Type Definitions
     *
     */
    typedef std::vector<std::vector<Pixel>> Screen;
    typedef std::vector<GenericDisplayObject *> LayoutObjects;
    typedef std::map<std::string, GenericDisplayObject *> ObjTable;
    typedef std::wstring Style;

    /**
     * @brief Direction of a Layout object, Horizontal or Vertical.
     *
     */
    enum Direction
    {
        Horizontal,
        Vertical
    };

    /**
     * @brief A class that provides basic attributes of styling.
     *
     */
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

    /**
     * @brief A class that represents each styled printed character on the screen.
     *
     */
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

    /**
     * @brief A struct that consists a pair of integers that represents a location coordinate.
     *
     */
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

    /**
     * @brief A struct that consists of a pair of integers that represents width and height.
     *
     */
    struct Size
    {
        bool operator==(Size &that);

        int width = 0, height = 0;
    };
}