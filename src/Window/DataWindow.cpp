#include "DataWindow.h"

namespace Xcurse
{
    /*
        Chart Window Data implementations
    */
    ChartWindowData::ChartWindowData() {}

    ChartWindowData::ChartWindowData(int w, int h, int h_offset, const Stylable &s) : width(w), height(h), height_offset(h_offset), style(s) {}

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
        m_data_vals = v_vals;
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
        // clear the data in buffer
        m_windata.clear();

        // sample data to be displayed
        int sample_size = std::min(static_cast<int>(m_data_vals.size()), get_width());

        if (sample_size > 0)
        {
            float max_val = *std::max_element(m_data_vals.begin(), m_data_vals.end());

            std::vector<ChartWindowData> samples;

            int unit_width = get_width() / sample_size;

            for (int i = m_data_vals.size() - sample_size; i < m_data_vals.size(); i++)
            {
                samples.emplace_back(ChartWindowData(
                    unit_width,
                    m_data_vals[i] * get_height() / max_val,
                    0,
                    Stylable(TEXT_COLOR_RESET, (i == 0 || m_data_vals[i] >= m_data_vals[i - 1]) ? BACKGROUND_COLOR_GREEN : BACKGROUND_COLOR_RED, false, false, false)));
            }

            // add pixels to buffer
            for (int i = 0; i < samples.size(); i++)
            {

                int _w = unit_width * i;

                for (int i_w = 0; i_w < samples[i].width; i_w++)
                {
                    for (int i_h = 0; i_h < samples[i].height; i_h++)
                    {
                        add_char(_w + i_w, i_h, L' ', samples[i].style);
                    }
                }
            }
        }

        GridWindow::draw();
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

    void TrendChartWindow::set_data(std::vector<float> &v_high, std::vector<float> &v_low)
    {
        // check if array size matches
        if (v_high.size() == v_low.size())
        {
            m_data_high = v_high;
            m_data_low = v_low;
        }
    }

    void TrendChartWindow::set_inc_style(const Style &s)
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