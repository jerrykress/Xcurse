#include "Line.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Line:: Line object
     *
     */
    Line::Line() : begin(Position{0, 0}), end(Position{1, 1}) {}

    /**
     * @brief Construct a new Line:: Line object
     *
     * @param begin
     * @param end
     */
    Line::Line(Position begin, Position end) : begin(begin), end(end) {}

    /**
     * @brief Rasterise the line and return the result stored internally
     *
     * @return std::vector<Position>&
     */
    std::vector<Position> &Line::rasterise()
    {
        const int x_diff = end.x - begin.x;
        const int y_diff = end.y - begin.y;
        const float steps = std::max(std::abs(x_diff), std::abs(y_diff));

        float unit_x = x_diff / steps;
        float unit_y = y_diff / steps;

        for (int i = 0; i <= steps; i++)
        {
            m_rasterised_data.emplace_back(Position{
                                               static_cast<int>(std::round(unit_x * i)),
                                               static_cast<int>(std::round(unit_y * i))} +
                                           begin);
        }
        return m_rasterised_data;
    }

    /**
     * @brief Set the begin point of the line
     *
     * @param p
     */
    void Line::set_begin(Position p)
    {
        begin = p;
    }

    /**
     * @brief Set the end point of the line
     *
     * @param p
     */
    void Line::set_end(Position p)
    {
        end = p;
    }

    /**
     * @brief Set the begin and the end of the line
     *
     * @param a
     * @param b
     */
    void Line::set_begin_end(Position a, Position b)
    {
        begin = a;
        end = b;
    }
}