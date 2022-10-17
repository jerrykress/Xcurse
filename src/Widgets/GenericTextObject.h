#pragma once

#include "../Display/Display.h"
#include "../Utils/Converter.h"

namespace Xcurse
{
    enum Alignment
    {
        ALIGN_LEFT,
        ALIGN_CENTER,
        ALIGN_RIGHT
    };

    class GenericTextObject : public BaseDisplayObject, public Stylable
    {
    public:
        GenericTextObject();

        Alignment alignment = ALIGN_CENTER;
    };
}