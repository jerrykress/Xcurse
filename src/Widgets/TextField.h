#include "GenericTextObject.h"

namespace Xcurse
{
    class TextField : public GenericTextObject
    {
    public:
        TextField();
        TextField(std::string name, const std::wstring &ws, Alignment a = ALIGN_CENTER);

    protected:
        void draw() override;
    };
}