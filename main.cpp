#include "src/Xcurse.h"

using namespace Xcurse;
using namespace std::literals::chrono_literals;

int main(int, char **)
{
    static bool prog_exit = false;

    Display::init();
    Display &d = *Display::get_display();
    d.set_refresh_interval(500);
    d.set_min_screen_size(Size{20, 20});
    d.enable_alt_screen(true);
    d.set_io_kb(true);

    d.add_obj("root", "t1", new TextField("t1", "Program", ALIGN_CENTER));
    d.add_obj("root", "v1", new Layout("v1", Horizontal, 1));
    d.add_obj("root", "w1", new TrendChartWindow("w1", 1));
    d.add_obj("v1", "w2", new GridWindow("w2", 1));
    d.add_obj("v1", "mline", new MultiTextField("mline", std::vector<std::wstring>{L"some text line 1", L"some text line 2", L"some text line 3"}, 1, ALIGN_CENTER));

    // setup win
    auto text_widget = static_cast<TextField *>(d["t1"]);
    text_widget->background = BACKGROUND_COLOR_BLUE;
    text_widget->bold = true;

    // setup win
    TrendChartWindow *trend_win = static_cast<TrendChartWindow *>(d["w1"]);
    std::vector<float> d_low = {1, 1, 3, 0, 0};
    std::vector<float> d_high = {2, 3, 5, 4, 2};
    std::vector<float> d_ref = {1.5, 2.0, 4.0, 2.0, 1.0};
    trend_win->set_data(d_low, d_high, d_ref);
    trend_win->set_title(L" Trend ");

    // setup win
    GridWindow *win = static_cast<GridWindow *>(d["w2"]);
    win->background = BACKGROUND_COLOR_BRIGHT_YELLOW;
    win->set_title(L" Analog ");

    // adding a keymap to power off
    d.map_key_action('x', [&]() -> void
                     { prog_exit = true; });
    d.map_key_action('g', [&]() -> void
                     { win->set_visible(); });

    d.power_on();

    int rotation = 0;
    Line line;
    Circle circle;
    Ellipse ellipse;

    while (!prog_exit)
    {
        win->clean();
        win->add_char(0, 0, L'\u0444', TEXT_COLOR_BLUE, BACKGROUND_COLOR_RED);

        Position win_center = Position{win->get_width() / 2, win->get_height() / 2};

        // test circle
        const int max_r = std::min(win->get_width(), win->get_height()) / 2 - 2;
        circle.set_midpoint(win_center);
        circle.set_radius(max_r);
        win->add_chars(circle.rasterise(), L'*');

        // test line
        line.set_begin_end(win_center, circle.get_at_degree(rotation));
        win->add_chars(line.rasterise(), L'x');

        // test ellipse
        ellipse.set_ra_rb(max_r * 2, max_r);
        ellipse.set_midpoint(win_center);
        win->add_chars(ellipse.rasterise(), L'@');

        // test line
        line.set_begin_end(win_center, ellipse.get_at_degree(rotation + 90));
        win->add_chars(line.rasterise(), L'&');

        rotation = (rotation + 6) % 360;
        std::this_thread::sleep_for(1s);
    }

    d.power_off();

    return 0;
}