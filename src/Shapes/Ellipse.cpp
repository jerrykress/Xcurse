#include "Ellipse.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Ellipse:: Ellipse object
     *
     */
    Ellipse::Ellipse() : ra(1), rb(1), midpoint(Position{0, 0}) {}

    /**
     * @brief Construct a new Ellipse:: Ellipse object
     *
     * @param midpoint
     * @param ra half width in x direction
     * @param rb half height in y direction
     */
    Ellipse::Ellipse(Position midpoint, int ra, int rb) : ra(ra), rb(rb), midpoint(midpoint) {}

    /**
     * @brief Rasterise the ellipse into points and return reference
     *
     * @return std::vector<Position>&
     */
    std::vector<Position> &Ellipse::rasterise()
    {
        m_rasterised_data.clear();

        // more suited to wider ellipses than taller
        const int range = std::max(ra, rb);
        const int b_sq = rb * rb;
        const int ab_sq = ra * ra * b_sq;

        for (int x = 0; x <= range; x++)
        {
            const int y = static_cast<int>(
                std::round(std::sqrt(ab_sq - b_sq * x * x) / (ra * 1.0)));

            const int temp_x = (ra >= rb) ? x : y;
            const int temp_y = (ra >= rb) ? y : x;

            m_rasterised_data.emplace_back(Position{temp_x, temp_y} + midpoint);
            m_rasterised_data.emplace_back(Position{temp_x, -temp_y} + midpoint);
            m_rasterised_data.emplace_back(Position{-temp_x, temp_y} + midpoint);
            m_rasterised_data.emplace_back(Position{-temp_x, -temp_y} + midpoint);
        }

        return m_rasterised_data;
    }

    /**
     * @brief Set the half width in x direction
     *
     * @param a
     */
    void Ellipse::set_ra(int a)
    {
        ra = a;
    }

    /**
     * @brief Set the half height in y direction
     *
     * @param b
     */
    void Ellipse::set_rb(int b)
    {
        rb = b;
    }

    /**
     * @brief Set both width and height of ellipse
     *
     * @param a
     * @param b
     */
    void Ellipse::set_ra_rb(int a, int b)
    {
        ra = a;
        rb = b;
    }

    /**
     * @brief Set midpoint of ellipse
     *
     * @param p
     */
    void Ellipse::set_midpoint(Position p)
    {
        midpoint = p;
    }

    /**
     * @brief Get a point on the ellipse at an radian angle from positive y axis
     *
     * @param a
     * @return Position
     */
    Position Ellipse::get_at_angle(float a)
    {
        a += 0.5 * pi;

        while (a >= 2 * pi)
        {
            a -= 2 * pi;
        }

        const float a_sq = ra * ra;
        const float b_sq = rb * rb;
        const float tan_t = std::tan(a);
        const float tan_sq = tan_t * tan_t;

        const float x = (ra * rb * 1.0) / std::sqrt(b_sq + a_sq * tan_sq);
        const float y = x * tan_t;

        const int sign = (a > 0.5 * pi && a < 1.5 * pi) ? -1 : 1;

        return Position{
                   static_cast<int>(std::round(x)) * -sign,
                   static_cast<int>(std::round(y)) * sign} +
               midpoint;
    }

    /**
     * @brief Get a point on the ellipse at an angle in degrees from positive y axis
     *
     * @param d
     * @return Position
     */
    Position Ellipse::get_at_degree(float d)
    {
        return get_at_angle(d * pi / 180.0);
    }

}