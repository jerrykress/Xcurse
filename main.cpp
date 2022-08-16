#include <iostream>
#include "src/xcurse.h"
#include "src/logger.h"
#include <thread>

using namespace Xcurse;
using namespace Console;
using namespace std::literals::chrono_literals;

int main(int, char **)
{
    Display *d = Display::get_display();
    Display::init();

    bool add_win0 = d->add_obj("root", "v1", new Layout("v1", Horizontal, 1));
    bool add_win3 = d->add_obj("root", "w1", new Window("w1", 1));
    // bool add_win4 = d->add_obj("root", "w4", new Window("w4", 1, "--||**##"));
    bool add_win1 = d->add_obj("v1", "w2", new Window("w2", 1));
    bool add_win2 = d->add_obj("v1", "w3", new Window("w3", 1));

    d->power_on();

    Window *win = static_cast<Window *>(d->get_obj("w2"));
    Window *winb = static_cast<Window *>(d->get_obj("w1"));

    for (int i = 0; i < 5; i++)
    {
        win->add_chars({std::make_tuple(i, i, L'\u2611')});
        winb->add_str(1, 1, L"test");
        // win->clear_buffer();
        std::this_thread::sleep_for(1s);
    }
    // std::cin.get();
    d->power_off();

    return 0;
}