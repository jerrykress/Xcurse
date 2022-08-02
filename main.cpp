#include <iostream>
#include "src/xcurse.h"
#include <thread>

using namespace Xcurse;
using namespace std::literals::chrono_literals;

int main(int, char **)
{

    Display *display = Display::get_display();

    auto size = display->get_size();
    display->set_refresh_interval(500);

    display->add_win(new StaticWindow("win", 0, 0, 10, 5, "--||**##"));

    auto win1 = display->get_window("win");

    display->start();

    for (int i = 0; i < 8; i += 2)
    {
        win1->add_str(0, 2, "String");
        win1->add_char(0, 0, '0' + i);
        win1->set_x(i);
        win1->set_y(i);
        std::this_thread::sleep_for(1s);
    }

    display->remove_win("win");

    display->add_win(new StaticWindow("win", 0, 0, 5, 10, "--||**##"));

    auto win2 = display->get_window("win");

    for (int i = 0; i < 8; i += 2)
    {
        win2->add_str(0, 2, "String");
        win2->add_char(0, 0, '0' + i);
        win2->set_x(i);
        win2->set_y(i);
        std::this_thread::sleep_for(1s);
    }
    std::cin.get();
    display->poweroff();
}