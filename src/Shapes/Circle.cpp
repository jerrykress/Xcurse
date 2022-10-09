#include "Circle.h"

namespace Xcurse
{
    /**
     * @brief Construct a new Circle:: Circle object
     *
     * @param midponit centre of the circle
     * @param radius radius of circle
     * @param restraint size of the container object if the circle needs to be cropped
     */
    Circle::Circle(Position midponit, int radius, Size restraint) : midpoint(midpoint), radius(radius), restraint(restraint) {}

    /**
     * @brief Construct a new Circle:: Circle object with maximum radius available, determined by the restraint imposed by the container
     *
     * @param restraint size of container object
     */
    Circle::Circle(Size restraint) : restraint(restraint) {}

    void Circle::rasterise()
    {
        get_rasterised(m_points);
    }

    void Circle::rasterise_styled()
    {
        get_rasterised_styled(m_pixels);
    }

    void Circle::get_rasterised(std::vector<Position> &target) const
    {
        const int r_sq = radius * radius;

        int range = std::sqrt(r_sq / 2) + 1;

        for (int x = 0; x <= range; x++)
        {
            int y = std::round(std::sqrt(r_sq - x * x));

            target.emplace_back(Position{x, y} + midpoint);
            target.emplace_back(Position{x, -y} + midpoint);
            target.emplace_back(Position{-x, y} + midpoint);
            target.emplace_back(Position{-x, -y} + midpoint);
            target.emplace_back(Position{y, x} + midpoint);
            target.emplace_back(Position{y, -x} + midpoint);
            target.emplace_back(Position{-y, x} + midpoint);
            target.emplace_back(Position{-y, -x} + midpoint);
        };
    }

    void Circle::get_rasterised_styled(std::vector<Pixel> &target) const
    {
        return;
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
     * @brief Set a new restraint for the circle
     * @note Setting to {0,0} means no restraint
     *
     * @param res
     */
    void Circle::set_restraint(Size res)
    {
        restraint = res;
    }
}