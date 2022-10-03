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

        void set_data(const std::string &s);
        void set_data(const std::wstring &ws);

        Alignment alignment = ALIGN_CENTER;

    protected:
        std::wstring m_data = L"";
    };
}