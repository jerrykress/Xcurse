#include "xcurse.h"

using namespace Xcurse;

class DataWindow : public GenericWindowObject
{
public:
    DataWindow();

    void show_legends(bool b);
    void show_color(bool b);

    void set_up_char(wchar_t c);
    void set_down_char(wchar_t c);

private:
}