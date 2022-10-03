#include "GenericTextObject.h"

namespace Xcurse
{
    GenericTextObject::GenericTextObject() {}

    void GenericTextObject::set_data(const std::wstring &ws)
    {
        m_data = ws;
    }

    void GenericTextObject::set_data(const std::string &s)
    {
        m_data = to_wstring(s);
    }
}