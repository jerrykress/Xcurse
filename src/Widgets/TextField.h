#pragma once

#include "GenericTextObject.h"

namespace Xcurse
{
    class TextField : public GenericTextObject
    {
    public:
        TextField();
        explicit TextField(std::string name, const std::string &s, Alignment a = ALIGN_CENTER);
        explicit TextField(std::string name, const std::wstring &ws, Alignment a = ALIGN_CENTER);

    protected:
        void draw() override;
    };
}