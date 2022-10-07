#include "Common.h"

namespace Xcurse
{
    /**
     * @brief Opearator <
     *
     * @param that
     * @return true
     * @return false
     */
    bool Position::operator<(const Position &that) const
    {
        return (y == that.y) ? (x < that.x) : (y < that.y);
    }

    /**
     * @brief Operator ==
     *
     * @param that
     * @return true
     * @return false
     */
    bool Position::operator==(const Position &that) const
    {
        return (x == that.x) && (y == that.y);
    }

    /**
     * @brief Operator +=
     *
     * @param that
     * @return Position&
     */
    Position &Position::operator+=(const Position &that)
    {
        x += that.x;
        y += that.y;
        return *this;
    }

    /**
     * @brief Operator ==
     *
     * @param that
     * @return true
     * @return false
     */
    bool Size::operator==(Size &that)
    {
        return width == that.width && height == that.height;
    }

    /**
     * @brief Less operator, return true if height or width is smaller
     *
     * @param that
     * @return true
     * @return false
     */
    bool Size::operator<(Size &that)
    {
        return width < that.width || height < that.height;
    }

    /**
     * @brief Greater operator, return true if both width and height are greater
     *
     * @param that
     * @return true
     * @return false
     */
    bool Size::operator>(Size &that)
    {
        return width > that.width && height > that.height;
    }

    /**
     * @brief Overload wide ostream
     *
     * @param wos
     * @param that
     * @return std::wostream&
     */
    std::wostream &operator<<(std::wostream &wos, const Size &that)
    {
        return wos << std::to_wstring(that.width) << L"x" << std::to_wstring(that.height) << std::endl;
    }
}