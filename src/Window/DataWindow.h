#include "GridWindow.h"

namespace Xcurse
{

    class TrendChartData
    {
        std::vector<float> v_high, v_low;
    };

    class BarChartData
    {
        std::vector<float> v_vals;
    };

    class DataWindow : public GridWindow
    {
    public:
        enum Window_Type
        {
            BAR_CHART,
            TREND_CHART
        };

        DataWindow();

        DataWindow(std::string name, int weight, std::wstring border = DEFAULT_WIN_BORDER, Window_Type t = BAR_CHART);

        void set_up_char(wchar_t c);
        void set_down_char(wchar_t c);

        void set_data(BarChartData &d);
        void set_data(TrendChartData &d);

    protected:
        Window_Type m_type;
        void draw() override;
    };

}
