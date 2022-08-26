#include "TextField.h"

namespace Xcurse
{
    TextField::TextField() {}

    TextField::TextField(const std::wstring &ws, Alignment a)
    {
        m_data = ws;
        alignment = a;
    }

    void TextField::draw()
    {
        if (alignment == ALIGN_LEFT)
        {
        }
        else if (alignment == ALIGN_CENTER)
        {
        }
        else if (alignment == ALIGN_RIGHT)
        {
        }
    }
}