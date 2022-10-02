// #include "../Display/Display.h"
#include "Common.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Stylable:: Stylable object
     *
     */
    Stylable::Stylable()
    {
        foreground = TEXT_COLOR_RESET;
        background = BACKGROUND_COLOR_RESET;
        bold = false;
        underline = false;
        reversed = false;
    }

    /**
     * @brief Construct a new Stylable:: Stylable object
     *
     * @param foreground Foreground color
     * @param background Background color
     * @param bold Apply bold style
     * @param underline Apply underline style
     * @param reversed Apply reversed color style
     */
    Stylable::Stylable(Style foreground, Style background, bool bold, bool underline, bool reversed) : foreground(foreground), background(background), bold(bold), underline(underline), reversed(reversed) {}

    /**
     * @brief Construct a new Stylable:: Stylable object
     *
     * @param that The Stylable object to be copied
     */
    Stylable::Stylable(const Stylable &that)
    {
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    /**
     * @brief Construct a new Stylable:: Stylable object
     *
     * @param that The Stylable object to be moved
     */
    Stylable::Stylable(Stylable &&that)
    {
        foreground = that.foreground;
        background = that.background;
        bold = that.bold;
        underline = that.underline;
        reversed = that.reversed;
    }

    /**
     * @brief Copy assign a Stylable object
     *
     * @param that The Stylable object to be copied
     * @return Reference of the new object
     */
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

    /**
     * @brief Move assign a Stylable object
     *
     * @param that The Stylable object to be moved
     * @return Reference of the new object
     */
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

    /**
     * @brief Destroy the Stylable:: Stylable object
     *
     */
    Stylable::~Stylable() {}

    /**
     * @brief Apply styling in stream
     *
     * @return A string that contains the styling print sequences
     */
    std::wstring Stylable::style() const
    {
        return foreground + background + (bold ? TEXT_STYLE_BOLD : L"") + (underline ? TEXT_STYLE_UNDERLINE : L"") + (reversed ? TEXT_STYLE_REVERSED : L"");
    }
}