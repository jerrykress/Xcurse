#include "DataWindow.h"

namespace Xcurse
{
    DataWindow::DataWindow()
    {
        m_type = Window_Type::BAR_CHART;
    }

    DataWindow::DataWindow(std::string name, int weight, std::wstring border, Window_Type t)
    {
        _name = name;
        m_border = border;
        m_weight = weight;
        m_display_ptr = Display::get_display();
        m_type = t;
    }

    void DataWindow::set_data(BarChartData &d)
    {
    }

    void DataWindow::set_data(TrendChartData &d)
    {
    }

    void DataWindow::draw()
    {
    }
}