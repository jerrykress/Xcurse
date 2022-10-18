#pragma once

#include "../Display/Display.h"
#include "../Utils/Converter.h"

namespace Xcurse
{
    /**
     * @brief Alignment of text in a text object
     *
     */
    enum Alignment
    {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

    /**
     * @brief Base template of all display objects that are used for displaying text
     *
     */
    class GenericTextObject : public BaseDisplayObject, public Stylable
    {
    public:
        GenericTextObject();

        Alignment alignment = ALIGN_CENTER;
    };
}