#pragma once

#include "../Display/Display.h"

namespace Xcurse
{
    // safe area definition
    struct SafeArea
    {
        int left, right, up, down;
    };

    enum SafeAreaDirection
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class GenericWindowObject : public BaseDisplayObject, public Stylable
    {
    public:
        GenericWindowObject();

        // getters
        int get_height() const override;
        int get_width() const override;
        Size get_size() const override;

        // setters
        void set_title(std::wstring title);
        void set_show_border(bool b);
        void set_show_titlebar(bool b);
        void set_override_win_style(bool b);
        void set_safe_area(SafeAreaDirection d, int v);
        void use_safe_area(bool b);

    protected:
        void draw_border();
        void draw_titlebar();

        std::wstring m_title = L" Title ";
        std::wstring m_border = DEFAULT_WIN_BORDER;
        bool m_override_win_style = false;
        bool m_show_border = true;
        bool m_show_titlebar = true;
        bool m_disable_safe_area = false;
        SafeArea m_safe_area = {1, 1, 1, 1};
    };
}