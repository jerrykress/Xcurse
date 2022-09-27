#include <iostream>
#include "src/Window/GridWindow.h"
#include <thread>

using namespace Xcurse;
using namespace std::literals::chrono_literals;

int main(int, char **)
{
    Display::init();
    Display &d = *Display::get_display();
    d.set_refresh_interval(200);

    bool add_win0 = d.add_obj("root", "v1", new Layout("v1", Horizontal, 1));
    bool add_win3 = d.add_obj("root", "w1", new GridWindow("w1", 1));
    bool add_win1 = d.add_obj("v1", "w2", new GridWindow("w2", 1));
    bool add_win2 = d.add_obj("v1", "w3", new GridWindow("w3", 1));

    // adding a keymap to power off
    d.map_key_action('x', [&]() -> void
                     { d.power_off(); });

    d.power_on();

    GridWindow *win = static_cast<GridWindow *>(d["w2"]);
    win->background = BACKGROUND_COLOR_BRIGHT_YELLOW;

    while (d.has_power())
    {
        win->add_char(2, 2, L'\u0444', TEXT_COLOR_BLUE, BACKGROUND_COLOR_RED);
        win->add_char(3, 3, d.get_key_press(), TEXT_COLOR_BLUE, BACKGROUND_COLOR_RED);
        std::this_thread::sleep_for(1s);
    }

    d.power_off();

    std::cout << "Finished with exit code 0\n";

    return 0;
}

// TODO: flexible and fixed size when render
// TODO: find if possible to terminate main thread from display