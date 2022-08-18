#include "xcurse.h"

namespace Xcurse
{
    Stylable::Stylable() {}

    Stylable::Stylable(Style foreground, Style background, bool bold, bool underline, bool reversed) : foreground(foreground), background(background), bold(bold), underline(underline), reversed(reversed) {}

    Stylable::Stylable(const Stylable &that)
    {
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    Stylable::Stylable(Stylable &&that)
    {

        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    Stylable &Stylable::operator=(const Stylable &that) noexcept
    {
        if (this != &that)
        {
            foreground = that.foreground;
            background = that.background;
            bold = that.bold;
            underline = that.underline;
            reversed = that.reversed;
        }
        return *this;
    }

    Stylable &Stylable::operator=(Stylable &&that) noexcept
    {

        if (this != &that)
        {
            foreground = that.foreground;
            background = that.background;
            bold = that.bold;
            underline = that.underline;
            reversed = that.reversed;
        }
        return *this;
    }

    Stylable::~Stylable() {}

    std::wstring Stylable::style() const
    {
        return foreground + background + (bold ? TEXT_STYLE_BOLD : L"") + (underline ? TEXT_STYLE_UNDERLINE : L"") + (reversed ? TEXT_STYLE_REVERSED : L"");
    }
}