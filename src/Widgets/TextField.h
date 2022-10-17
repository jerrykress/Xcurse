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

        void set_data(const std::string &s);
        void set_data(const std::wstring &ws);

    protected:
        void draw() override;
        std::wstring m_data = L"";
    };
}