#include "GenericTextObject.h"

namespace Xcurse
{
    GenericTextObject::GenericTextObject() {}

    void GenericTextObject::draw() {}

    void GenericTextObject::set_data(const std::wstring &ws)
    {
        m_data = ws;
    }
}