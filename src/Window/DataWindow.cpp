#include "DataWindow.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Chart Window Data:: Chart Window Data object
     *
     */
    ChartWindowData::ChartWindowData() {}

    ChartWindowData::ChartWindowData(int w, int h, int h_offset, const Stylable &s) : width(w), height(h), height_offset(h_offset), style(s) {}

    /**
     * @brief Construct a new Bar Chart Window:: Bar Chart Window object
     *
     */
    BarChartWindow::BarChartWindow()
    {
        _name = "Untitled";
        m_weight = 1;
        m_border = DEFAULT_WIN_BORDER;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
        m_display_ptr = Display::get_display();
    }

    /**
     * @brief Construct a new Bar Chart Window:: Bar Chart Window object
     *
     * @param name
     * @param weight
     * @param border
     */
    BarChartWindow::BarChartWindow(std::string name, int weight, std::wstring border)
    {
        _name = name;
        m_weight = weight;
        m_border = border;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
        m_display_ptr = Display::get_display();
    }

    /**
     * @brief Set the data of bar chart window
     *
     * @param v_vals Value of data
     */
    void BarChartWindow::set_data(std::vector<float> &v_vals)
    {
        m_data_vals = v_vals;
    }

    /**
     * @brief Set the color of bar when there is an increase in value
     *
     * @param s
     */
    void BarChartWindow::set_inc_style(const Style &s)
    {
        m_inc_style = s;
    }

    /**
     * @brief Set the color of bar when there is a decrease in value
     *
     * @param s
     */
    void BarChartWindow::set_dec_style(const Style &s)
    {
        m_dec_style = s;
    }

    /**
     * @brief Sample from raw data and draw the bars to display buffer
     *
     */
    void BarChartWindow::draw()
    {
        // clear the data in buffer
        m_windata.clear();

        // sample data to be displayed
        int sample_size = std::min(static_cast<int>(m_data_vals.size()), get_width());

        if (sample_size > 0)
        {
            // find max value in all data points
            float max_val = *std::max_element(m_data_vals.begin(), m_data_vals.end());

            // sample adequate number of data to build bars
            std::vector<ChartWindowData> samples;

            int unit_width = get_width() / sample_size;

            // build each bar
            for (int i = m_data_vals.size() - sample_size; i < m_data_vals.size(); i++)
            {
                samples.emplace_back(ChartWindowData(
                    unit_width,
                    std::max((int)std::round(m_data_vals[i] * get_height() / max_val), 1),
                    0,
                    Stylable(TEXT_COLOR_RESET, (i == 0 || m_data_vals[i] >= m_data_vals[i - 1]) ? m_inc_style : m_dec_style, false, false, false)));
            }

            // add pixels to buffer
            for (int i = 0; i < samples.size(); i++)
            {
                // starting width position
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

    /**
     * @brief Construct a new Trend Chart Window:: Trend Chart Window object
     *
     */
    TrendChartWindow::TrendChartWindow()
    {
        _name = "Untitled";
        m_weight = 1;
        m_border = DEFAULT_WIN_BORDER;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
        m_display_ptr = Display::get_display();
    }

    /**
     * @brief Construct a new Trend Chart Window:: Trend Chart Window object
     *
     * @param name
     * @param weight
     * @param border
     */
    TrendChartWindow::TrendChartWindow(std::string name, int weight, std::wstring border)
    {
        _name = name;
        m_weight = weight;
        m_border = border;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
        m_display_ptr = Display::get_display();
    }

    /**
     * @brief Set the data of trend chart window
     *
     * @param v_open Open values of trend
     * @param v_close Close values of trend
     */
    void TrendChartWindow::set_data(std::vector<float> &v_open, std::vector<float> &v_close)
    {
        // check if array size matches
        if (v_open.size() == v_close.size())
        {
            m_data_open = v_open;
            m_data_close = v_close;
        }
    }

    /**
     * @brief Set the color of bar when there is an increase in value
     *
     * @param s
     */
    void TrendChartWindow::set_inc_style(const Style &s)
    {
        m_inc_style = s;
    }

    /**
     * @brief Set the color of bar when there is a decrease in value
     *
     * @param s
     */
    void TrendChartWindow::set_dec_style(const Style &s)
    {
        m_dec_style = s;
    }

    /**
     * @brief Sample from raw data and draw the bars to display buffer
     *
     */
    void TrendChartWindow::draw()
    {
        // clear the data in buffer
        m_windata.clear();

        // sample data to be displayed
        int sample_size = std::min(static_cast<int>(m_data_open.size()), get_width());

        if (sample_size > 0)
        {
            // find max value in all data points
            float max_val = std::max(
                                *std::max_element(m_data_open.begin(), m_data_open.end()),
                                *std::max_element(m_data_close.begin(), m_data_close.end())) *
                            DATA_MAGNIF_FACTOR;

            // find min value in all data points
            float min_val = std::min(
                                *std::min_element(m_data_open.begin(), m_data_open.end()),
                                *std::min_element(m_data_close.begin(), m_data_close.end())) *
                            DATA_MAGNIF_FACTOR;

            // diff between max and min val
            const float max_diff = std::abs(max_val - min_val);

            // sample adequate number of data to build the bars
            std::vector<ChartWindowData> samples;

            // width of each bar
            int unit_width = get_width() / sample_size;

            // build each bar
            for (int i = m_data_open.size() - sample_size; i < m_data_open.size(); i++)
            {
                samples.emplace_back(ChartWindowData(
                    unit_width,
                    std::max((int)(std::abs(m_data_open[i] * DATA_MAGNIF_FACTOR - m_data_close[i] * DATA_MAGNIF_FACTOR) * get_height() / max_diff), 1),
                    (std::min(m_data_open[i], m_data_close[i]) * DATA_MAGNIF_FACTOR - min_val) * get_height() / max_diff,
                    Stylable(TEXT_COLOR_RESET, (m_data_close[i] > m_data_open[i]) ? m_inc_style : m_dec_style, false, false, false)));
            }

            // add pixels to buffer
            for (int i = 0; i < samples.size(); i++)
            {
                // starting width position
                int _w = unit_width * i;

                for (int i_w = 0; i_w < samples[i].width; i_w++)
                {
                    for (int i_h = 0; i_h < samples[i].height; i_h++)
                    {
                        add_char(_w + i_w, i_h + samples[i].height_offset, L' ', samples[i].style);
                    }
                }
            }
        }

        GridWindow::draw();
    }
}