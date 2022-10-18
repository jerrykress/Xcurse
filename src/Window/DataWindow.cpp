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
        m_weight = 1;
        m_border = DEFAULT_WIN_BORDER;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
    }

    /**
     * @brief Construct a new Bar Chart Window:: Bar Chart Window object
     *
     * @param weight
     * @param border
     */
    BarChartWindow::BarChartWindow(int weight, std::wstring border)
    {
        m_weight = weight;
        m_border = border;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
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
        m_weight = 1;
        m_border = DEFAULT_WIN_BORDER;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
    }

    /**
     * @brief Construct a new Trend Chart Window:: Trend Chart Window object
     *
     * @param weight
     * @param border
     */
    TrendChartWindow::TrendChartWindow(int weight, std::wstring border)
    {
        m_weight = weight;
        m_border = border;
        m_inc_style = BACKGROUND_COLOR_GREEN;
        m_dec_style = BACKGROUND_COLOR_RED;
    }

    /**
     * @brief Set the data of trend chart window
     *
     * @param v_low Open values of trend
     * @param v_high Close values of trend
     * @param v_ref Value to determine if trend is increase or decrease
     */
    void TrendChartWindow::set_data(std::vector<float> &v_low, std::vector<float> &v_high, std::vector<float> &v_ref)
    {
        // check if array sizes match
        if (v_low.size() == v_high.size() && v_high.size() == v_ref.size())
        {
            m_data_low = v_low;
            m_data_high = v_high;
            m_data_ref = v_ref;
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
        int sample_size = std::min(static_cast<int>(m_data_low.size()), get_width());

        if (sample_size > 0)
        {
            // find max value in all data points
            float max_val_scaled = *std::max_element(m_data_high.begin(), m_data_high.end()) * DATA_MAGNIF_FACTOR;

            // find min value in all data points
            float min_val_scaled = *std::min_element(m_data_low.begin(), m_data_low.end()) * DATA_MAGNIF_FACTOR;

            // diff between max and min val
            const float max_diff_scaled = std::abs(max_val_scaled - min_val_scaled);

            // sample adequate number of data to build the bars
            std::vector<ChartWindowData> samples;

            // width of each bar
            int unit_width = get_width() / sample_size;

            // build each bar
            for (int i = m_data_low.size() - sample_size; i < m_data_low.size(); i++)
            {
                samples.emplace_back(ChartWindowData(
                    unit_width,
                    std::max((int)(std::abs(m_data_high[i] * DATA_MAGNIF_FACTOR - m_data_low[i] * DATA_MAGNIF_FACTOR) * get_height() / max_diff_scaled), 1),
                    (m_data_low[i] * DATA_MAGNIF_FACTOR - min_val_scaled) * get_height() / max_diff_scaled,
                    Stylable(TEXT_COLOR_RESET, (i == 0 || m_data_ref[i] > m_data_ref[i - 1]) ? m_inc_style : m_dec_style, false, false, false)));
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