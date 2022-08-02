#include <iostream>
#include "src/xcurse.h"
#include <thread>

using namespace Xcurse;
using namespace std::literals::chrono_literals;

int main(int, char **)
{
    std::cout << "\e[?47h" << std::endl;

    Display *display = Display::get_display();

    auto size = display->get_size();

    display->add_win(new StaticWindow("win", 0, 0, 10, 5, "--||**##"));

    auto win1 = display->get_window("win");
    // display->refresh();

    for (int i = 0; i < 5; i++)
    {
        win1->add_char(0, 0, '0' + i);
        std::this_thread::sleep_for(1s);
        display->refresh();
    }

    std::cin.get();
    std::cout << "\e[?47l" << std::endl;
}
