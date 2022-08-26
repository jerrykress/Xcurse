#include "GenericTextObject.h"

namespace Xcurse
{
    class TextField : public GenericTextObject
    {
    public:
        TextField();
        TextField(const std::wstring &ws, Alignment a = ALIGN_CENTER);

        void set_multiline(bool b);

    protected:
        void draw() override;
    };
}