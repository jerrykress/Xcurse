#include <iostream>
#include "src/xcurse.h"
#include "src/logger.h"
#include <thread>

using namespace Xcurse;
using namespace Console;
using namespace std::literals::chrono_literals;

int main(int, char **)
{
    Display::init();
    Display &d = *Display::get_display();
    d.set_refresh_interval(200);

    bool add_win0 = d.add_obj("root", "v1", new Layout("v1", Horizontal, 1));
    bool add_win3 = d.add_obj("root", "w1", new GridWindow("w1", 1));
    // bool add_win4 = d->add_obj("root", "w4", new Window("w4", 1, "--||**##"));
    bool add_win1 = d.add_obj("v1", "w2", new GridWindow("w2", 1));
    bool add_win2 = d.add_obj("v1", "w3", new GridWindow("w3", 1));

    d.power_on();

    GridWindow *win = static_cast<GridWindow *>(d["w2"]);

    for (int i = 0; i < 5; i++)
    {
        win->add_char(i + 2, i + 2, L'\u0444', TEXT_COLOR_BLUE, BACKGROUND_COLOR_RED);
        std::this_thread::sleep_for(1s);
    }
    // std::cin.get();
    d.power_off();

    return 0;
}