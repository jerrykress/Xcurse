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
     * @param restraint size of the container object if the circle needs to be cropped
     */
    Circle::Circle(Position midponit, int radius, Size restraint) : midpoint(midpoint), radius(radius) {}

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

}