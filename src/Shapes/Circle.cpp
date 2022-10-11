#include "Circle.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Circle:: Circle object
     *
     */
    Circle::Circle() : midpoint(Position{0, 0}), radius(1) {}

    /**
     * @brief Construct a new Circle:: Circle object
     *
     * @param midponit centre of the circle
     * @param radius radius of circle
     */
    Circle::Circle(Position midponit, int radius) : midpoint(midpoint), radius(radius) {}

    /**
     * @brief Rasterise the circle, and returns the reference of the result stored internally
     *
     * @return std::vector<Position>&
     */
    std::vector<Position> &Circle::rasterise()
    {
        m_rasterised_data.clear();

        const int r_sq = radius * radius;

        int range = std::sqrt(r_sq / 2) + 1;

        for (int x = 0; x <= range; x++)
        {
            int y = std::round(std::sqrt(r_sq - x * x));

            m_rasterised_data.emplace_back(Position{x, y} + midpoint);
            m_rasterised_data.emplace_back(Position{x, -y} + midpoint);
            m_rasterised_data.emplace_back(Position{-x, y} + midpoint);
            m_rasterised_data.emplace_back(Position{-x, -y} + midpoint);
            m_rasterised_data.emplace_back(Position{y, x} + midpoint);
            m_rasterised_data.emplace_back(Position{y, -x} + midpoint);
            m_rasterised_data.emplace_back(Position{-y, x} + midpoint);
            m_rasterised_data.emplace_back(Position{-y, -x} + midpoint);
        };

        return m_rasterised_data;
    }

    /**
     * @brief Set a new radius of the circle
     *
     * @param r
     */
    void Circle::set_radius(int r)
    {
        radius = r;
    }

    /**
     * @brief Set a new midpoint of the circle
     *
     * @param p
     */
    void Circle::set_midpoint(Position p)
    {
        midpoint = p;
    }

    /**
     * @brief Get a point on the circle at an angle (radian)
     *
     * @param a
     * @return Position
     */
    Position Circle::get_at_angle(float a)
    {
        return Position{
                   static_cast<int>(std::round(radius * std::sin(a))),
                   static_cast<int>(std::round(radius * std::cos(a)))} +
               midpoint;
    }

    /**
     * @brief Get a point on the circle at an angle (degree)
     *
     * @param d
     * @return Position
     */
    Position Circle::get_at_degree(float d)
    {
        return get_at_angle(d * pi / 180.0);
    }

}