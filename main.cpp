#include <iostream>
#include "src/xcurse.h"

using namespace Xcurse;

int main(int, char **)
{
    std::cout << "\e[?1049h" << std::endl;

    Display *display = Display::get_display();

    auto size = display->get_size();

    StaticWindow sw("win", 10, 10, 10, 5, "--||**##");
    display->add_win(sw);
    display->refresh();

    std::cin.get();
    std::cout << "\e[?1049l" << std::endl;
}
