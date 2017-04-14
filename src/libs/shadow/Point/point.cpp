#include "point.h"

#include "../Vector/vector.h"
#include "../Bbox/bbox.h"

#include <CGAL/number_utils.h>

namespace urban
{
    namespace shadow
    {
        Point::Point(void)
            : coordinates{{0, 0, 0}} {}
        Point::Point(double x, double y, double z)
            : coordinates{{x, y, z}} {}
        Point::Point(double _coordinates[3])
            : coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}} {}
        Point::Point(const Point_3 & point)
            : coordinates{{CGAL::to_double(point.x()), CGAL::to_double(point.y()), CGAL::to_double(point.z())}} {}
        Point::Point(Lib3dsPoint const& point)
            : coordinates{{static_cast<double>(point.pos[0]), static_cast<double>(point.pos[1]), static_cast<double>(point.pos[2])}} {}
        Point::Point(Point const& other)
            : coordinates(other.coordinates) {}
        Point::Point(Point && other)
            : coordinates(std::move(other.coordinates)) {}
        Point::~Point(void) {}
        
        double Point::x(void) const noexcept
        {
            return coordinates.at(0);
        }
        double Point::y(void) const noexcept
        {
            return coordinates.at(1);
        }
        double Point::z(void) const noexcept
        {
            return coordinates.at(2);
        }

        void Point::swap(Point & other)
        {
            using std::swap;
            swap(coordinates, other.coordinates);
        }

        Point & Point::operator=(Point const& other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }


        Point & Point::operator=(Point && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Point & Point::operator+=(Vector const& translation)
        {
            coordinates.at(0) += translation.x();
            coordinates.at(1) += translation.y();
            coordinates.at(2) += translation.z();

            return *this;
        }

        Bbox Point::bbox(void) const
        {
            return Bbox(coordinates);
        }

        std::ostream & operator<<(std::ostream & os, Point const& point)
        {
            os << point.coordinates.at(0) << " " << point.coordinates.at(1) << " " << point.coordinates.at(2);
            return os;
        }
        
        Vector operator-(Point const& lhs, Point const& rhs)
        {
            return Vector(rhs, lhs);
        }

        Point & operator+(Point & lhs, Vector const& rhs)
        {
            return lhs += rhs;
        }

        bool operator==(Point const& lhs, Point const& rhs)
        {
            return (rhs - lhs) == shadow::Vector();
        }
        
        bool operator!=(Point const& lhs, Point const& rhs)
        {
            return !(rhs == lhs);
        }
    }

    void swap(shadow::Point & lhs, shadow::Point & rhs)
    {
        lhs.swap(rhs);
    }

    shadow::Vector normal_to(shadow::Point const& first, shadow::Point const& second, shadow::Point const& third)
    {
        shadow::Vector v(first, second);
        v ^= shadow::Vector(second, third);
        return v / norm_L2(v);
    }

}
