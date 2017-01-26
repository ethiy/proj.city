#include "projection_algorithms.h"

#include <vector>
#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    Affine_transformation rotation_transform(const std::map<double, Vector> & _rotations)
    {
        std::vector<Affine_transformation> rotations;
        double norm(0);
        Vector u(CGAL::NULL_VECTOR);
        std::transform(
            std::begin(_rotations),
            std::end(_rotations),
            std::begin(rotations),
            [norm, u](const std::pair<double, Vector> & angle_axis) mutable
            {
                norm = std::sqrt(to_double(angle_axis.second * angle_axis.second));
                u = angle_axis.second / norm;
                return Affine_transformation(
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.x()), 2.), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.y()) - std::sin(angle_axis.first) * to_double(u.z()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.z()) + std::sin(angle_axis.first) * to_double(u.y()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.y()) + std::sin(angle_axis.first) * to_double(u.z()), 
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.y()), 2.), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.y()) * to_double(u.z()) - std::sin(angle_axis.first) * to_double(u.x()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.z()) - std::sin(angle_axis.first) * to_double(u.y()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.y()) * to_double(u.z()) + std::sin(angle_axis.first) * to_double(u.x()), 
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.z()), 2.)
                    );
            }
        );

        return std::accumulate(
            std::begin(rotations),
            std::end(rotations),
            Affine_transformation_2(CGAL::IDENTITY),
            [](Affine_transformation_2 & result, const Affine_transformation_2 & rotation)
            {
                return result * rotation;
            }
        );
    }
}
