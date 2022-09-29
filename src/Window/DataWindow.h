#include "GridWindow.h"

namespace Xcurse
{
    class ChartWindowData
    {
    public:
        ChartWindowData();

        ChartWindowData(int w, int h, int h_offset, const Stylable &s);

        int width, height, height_offset;
        Stylable style;
    };

    class BarChartWindow : public GridWindow
    {
    public:
        BarChartWindow();

        BarChartWindow(std::string name, int weight = 1, std::wstring border = DEFAULT_WIN_BORDER);

        void set_data(std::vector<float> &v_val);
        void set_inc_style(const Style &s);
        void set_dec_style(const Style &s);

    protected:
        std::vector<float> m_data_vals;
        Style m_inc_style, m_dec_style;
        void draw() override;
    };

    class TrendChartWindow : public GridWindow
    {
    public:
        TrendChartWindow();

        TrendChartWindow(std::string name, int weight = 1, std::wstring border = DEFAULT_WIN_BORDER);

        void set_data(std::vector<float> &v_high, std::vector<float> &v_low);
        void set_inc_style(const Style &s);
        void set_dec_style(const Style &s);

    protected:
        std::vector<float> m_data_high, m_data_low;
        Style m_inc_style, m_dec_style;
        void draw() override;
    };

}
