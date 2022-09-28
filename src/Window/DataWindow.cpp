#include "DataWindow.h"

namespace Xcurse
{
    /*
        Bar Chart Window implementations
    */

    BarChartWindow::BarChartWindow()
    {
        _name = "Untitled";
        m_weight = 1;
        m_border = DEFAULT_WIN_BORDER;
        m_display_ptr = Display::get_display();
    }

    BarChartWindow::BarChartWindow(std::string name, int weight, std::wstring border)
    {
        _name = name;
        m_weight = weight;
        m_border = border;
        m_display_ptr = Display::get_display();
    }

    void BarChartWindow::set_data(std::vector<float> &v_vals)
    {
    }

    void BarChartWindow::set_inc_style(const Style &s)
    {
        m_inc_style = s;
    }

    void BarChartWindow::set_dec_style(const Style &s)
    {
        m_dec_style = s;
    }

    void BarChartWindow::draw()
    {
    }

    /*
        Trend Chart Window implementations
    */

    TrendChartWindow::TrendChartWindow()
    {

        _name = "Untitled";
        m_weight = 1;
        m_border = DEFAULT_WIN_BORDER;
        m_display_ptr = Display::get_display();
    }

    TrendChartWindow::TrendChartWindow(std::string name, int weight, std::wstring border)
    {
        _name = name;
        m_weight = weight;
        m_border = border;
        m_display_ptr = Display::get_display();
    }

    void BarChartWindow::set_inc_style(const Style &s)
    {
        m_inc_style = s;
    }

    void TrendChartWindow::set_dec_style(const Style &s)
    {
        m_dec_style = s;
    }

    void TrendChartWindow::draw()
    {
    }
}