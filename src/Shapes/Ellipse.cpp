#include "Ellipse.h"

namespace Xcurse
{
    Ellipse::Ellipse() : ra(1), rb(1), midpoint(Position{0, 0}) {}

    Ellipse::Ellipse(Position midpoint, int ra, int rb) : ra(ra), rb(rb), midpoint(midpoint) {}

    std::vector<Position> &Ellipse::rasterise()
    {
        const int range = std::max(ra, rb);
        const int b_sq = rb * rb;
        const int ab_sq = ra * ra * b_sq;

        for (int x = 0; x <= range; x++)
        {
            int y = static_cast<int>(
                std::round(std::sqrt(ab_sq - b_sq * x * x) / (ra * 1.0)));

            int temp_x = (ra >= rb) ? x : y;
            int temp_y = (ra >= rb) ? y : x;

            m_rasterised_data.emplace_back(Position{temp_x, temp_y} + midpoint);
            m_rasterised_data.emplace_back(Position{temp_x, -temp_y} + midpoint);
            m_rasterised_data.emplace_back(Position{-temp_x, temp_y} + midpoint);
            m_rasterised_data.emplace_back(Position{-temp_x, -temp_y} + midpoint);
        }

        return m_rasterised_data;
    }

    void Ellipse::set_ra(int a)
    {
        ra = a;
    }

    void Ellipse::set_rb(int b)
    {
        rb = b;
    }

    void Ellipse::set_ra_rb(int a, int b)
    {
        ra = a;
        rb = b;
    }

    void Ellipse::set_midpoint(Position p)
    {
        midpoint = p;
    }

    Position Ellipse::get_at_angle(float a)
    {
        // TODO:
        return Position{0, 0};
    }

    Position Ellipse::get_at_degree(float d)
    {
        // TODO:
        return Position{0, 0};
    }

}