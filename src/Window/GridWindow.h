#pragma once

#include "GenericWindowObject.h"

namespace Xcurse
{
    class GridWindow : public GenericWindowObject
    {
        typedef std::map<Position, Pixel> WindowData;

    public:
        GridWindow();

        GridWindow(int weight, std::wstring border = DEFAULT_WIN_BORDER);

        void add_char(int x, int y, wchar_t c, Stylable s = Stylable());

        void add_char(Position p, wchar_t c, Stylable s = Stylable());

        void add_char(int x, int y, wchar_t c, Style foreground = TEXT_COLOR_RESET, Style background = BACKGROUND_COLOR_RESET, bool bold = false, bool underline = false, bool reversed = false);

        void add_chars(std::vector<Position> &ps, wchar_t c, Stylable s = Stylable());

        bool clear_char(int x, int y);
        void clean();

    protected:
        WindowData m_windata;
        void draw() override;
    };
}