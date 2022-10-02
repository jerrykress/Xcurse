#include <iostream>
#include "src/Window/GridWindow.h"
#include "src/Window/DataWindow.h"
#include "src/Widgets/TextField.h"
#include <thread>

using namespace Xcurse;
using namespace std::literals::chrono_literals;

int main(int, char **)
{
    Display::init();
    Display &d = *Display::get_display();
    d.set_refresh_interval(200);

    bool add_wig0 = d.add_obj("root", "t1", new TextField("t1", L"Program", ALIGN_CENTER));
    bool add_win0 = d.add_obj("root", "v1", new Layout("v1", Horizontal, 1));
    bool add_win3 = d.add_obj("root", "w1", new TrendChartWindow("w1", 1));
    bool add_win1 = d.add_obj("v1", "w2", new GridWindow("w2", 1));
    bool add_win2 = d.add_obj("v1", "w3", new BarChartWindow("w3", 1));

    TrendChartWindow *trend_win = static_cast<TrendChartWindow *>(d["w1"]);
    std::vector<float> d_low = {1, 1, 3, 0, 0};
    std::vector<float> d_high = {2, 3, 5, 4, 2};
    std::vector<float> d_ref = {1.5, 2.0, 4.0, 2.0, 1.0};
    trend_win->set_data(d_low, d_high, d_ref);
    trend_win->set_title(L" Trend ");

    GridWindow *win = static_cast<GridWindow *>(d["w2"]);
    win->background = BACKGROUND_COLOR_BRIGHT_YELLOW;
    win->set_title(L" Grid ");

    BarChartWindow *bar_win = static_cast<BarChartWindow *>(d["w3"]);
    std::vector<float> ds = {1, 2, 3, 2, 1, 2, 3};
    bar_win->set_data(ds);
    bar_win->set_title(L" Bar ");

    // adding a keymap to power off
    d.map_key_action('x', [&]() -> void
                     { d.power_off(); });
    d.map_key_action('g', [&]() -> void
                     { win->set_visible(); });
    d.map_key_action('b', [&]() -> void
                     { bar_win->set_visible(); });

    d.power_on();

    while (d.has_power())
    {
        win->add_char(0, 0, L'\u0444', TEXT_COLOR_BLUE, BACKGROUND_COLOR_RED);
        win->add_char(3, 3, d.get_key_press(), TEXT_COLOR_BLUE, BACKGROUND_COLOR_RED);
        std::this_thread::sleep_for(1s);
    }

    d.power_off();

    std::cout << "Finished with exit code 0\n";

    return 0;
}

// TODO: flexible and fixed size when render
// TODO: find if possible to terminate main thread from display