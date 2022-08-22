#include "common.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Pixel:: Pixel object
     *
     */
    Pixel::Pixel() {}

    /**
     * @brief Construct a new Pixel:: Pixel object
     *
     * @param c The character that will be stored in the pixel
     */
    Pixel::Pixel(wchar_t c) : data(c) {}

    /**
     * @brief Construct a new Pixel:: Pixel object
     *
     * @param c The character that will be stored in the pixel
     * @param s A stylable object whose style will be applied to this Pixel
     */
    Pixel::Pixel(wchar_t c, Stylable &s) : data(c)
    {
        foreground = s.foreground;
        background = s.background;
        bold = s.bold;
        underline = s.underline;
        reversed = s.reversed;
    }

    /**
     * @brief Construct a new Pixel:: Pixel object
     *
     * @param c The character that will be stored in the pixel
     * @param s A stylable object whose style will be applied to this Pixel
     */
    Pixel::Pixel(wchar_t c, Stylable &&s) : data(c)
    {
        foreground = s.foreground;
        background = s.background;
        bold = s.bold;
        underline = s.underline;
        reversed = s.reversed;
    }

    /**
     * @brief Construct a new Pixel:: Pixel object
     *
     * @param that Pixel object to be copied
     */
    Pixel::Pixel(const Pixel &that)
    {
        data = that.data;
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    /**
     * @brief Construct a new Pixel:: Pixel object
     *
     * @param that Pixel object to be moved
     */
    Pixel::Pixel(Pixel &&that)
    {
        data = that.data;
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    /**
     * @brief Copy assign a Pixel object
     *
     * @param that Pixel object to be copied
     * @return Reference of the new Pixel object
     */
    Pixel &Pixel::operator=(const Pixel &that) noexcept
    {
        if (this != &that)
        {
            data = that.data;
            foreground = that.foreground;
            background = that.background;
            bold = that.bold;
            underline = that.underline;
            reversed = that.reversed;
        }
        return *this;
    }

    /**
     * @brief Move assign a Pixel object
     *
     * @param that Pixel object to be moved
     * @return Reference of the new Pixel object
     */
    Pixel &Pixel::operator=(Pixel &&that) noexcept
    {
        if (this != &that)
        {
            data = that.data;
            foreground = that.foreground;
            background = that.background;
            bold = that.bold;
            underline = that.underline;
            reversed = that.reversed;
        }
        return *this;
    }

    /**
     * @brief Destroy the Pixel:: Pixel object
     *
     */
    Pixel::~Pixel() {}

    /**
     * @brief Wide character stream overload for Pixel object
     *
     * @param out
     * @param px
     * @return std::wostream&
     */
    std::wostream &operator<<(std::wostream &out, const Pixel &px)
    {
        return out << px.style() << px.data;
    }
}