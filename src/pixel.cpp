#include "xcurse.h"

namespace Xcurse
{
    Pixel::Pixel() {}

    Pixel::Pixel(wchar_t c) : data(c) {}

    Pixel::Pixel(wchar_t c, Stylable &s) : data(c)
    {
        foreground = s.foreground;
        background = s.background;
        bold = s.bold;
        underline = s.underline;
        reversed = s.reversed;
    }

    Pixel::Pixel(wchar_t c, Stylable &&s) : data(c)
    {
        foreground = s.foreground;
        background = s.background;
        bold = s.bold;
        underline = s.underline;
        reversed = s.reversed;
    }

    Pixel::Pixel(const Pixel &that)
    {
        data = that.data;
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    Pixel::Pixel(Pixel &&that)
    {
        data = that.data;
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

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

    Pixel::~Pixel() {}

    std::wostream &operator<<(std::wostream &out, const Pixel &px)
    {
        return out << px.style() << px.data;
    }
}