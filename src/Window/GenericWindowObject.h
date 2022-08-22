#include "../Display/Display.h"

namespace Xcurse
{
    class GenericWindowObject : public BaseDisplayObject, public Stylable
    {
    public:
        GenericWindowObject();
        void set_title(std::wstring title);
        void set_show_border(bool b);
        void set_show_titlebar(bool b);
        void set_override_win_style(bool b);

    protected:
        void draw_border();
        void draw_titlebar();

        std::wstring m_title = L" Title ";
        std::wstring m_border = DEFAULT_WIN_BORDER;
        bool override_win_style = false, show_border = true, show_titlebar = true;
    };
}