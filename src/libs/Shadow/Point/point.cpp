#include "point.h"

#include "../Vector/vector.h"
#include "../Bbox/bbox.h"

#include <CGAL/number_utils.h>

namespace urban
{
    namespace shadow
    {
        Point::Point(void): coordinates{{0, 0, 0}} {}
        Point::Point(double x, double y, double z): coordinates{{x, y, z}} {}
        Point::Point(double _coordinates[3]): coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}} {}
        Point::Point(const Point_3 & point):coordinates{{CGAL::to_double(point.x()), CGAL::to_double(point.y()), CGAL::to_double(point.z())}} {}
        Point::Point(const Point & other): coordinates(other.coordinates) {}
        Point::Point(Point && other): coordinates(std::move(other.coordinates)) {}
        Point::~Point(void){}
        
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

        Point & Point::operator=(const Point & other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }


        Point & Point::operator=(Point && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Point & Point::operator+=(const Vector & translation)
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

        std::ostream & operator<<(std::ostream & os, const Point & point)
        {
            os << point.coordinates.at(0) << " " << point.coordinates.at(1) << " " << point.coordinates.at(2);
            return os;
        }
        
        Vector operator-(const Point & lhs, const Point & rhs)
        {
            return Vector(rhs, lhs);
        }

        Point & operator+(Point & lhs, const Vector & rhs)
        {
            return lhs += rhs;
        }

        bool operator==(const Point & lhs, const Point & rhs)
        {
            return (rhs - lhs) == shadow::Vector();
        }
        
        bool operator!=(const Point & lhs, const Point & rhs)
        {
            return !(rhs == lhs);
        }
    }

    void swap(shadow::Point & lhs, shadow::Point & rhs)
    {
        lhs.swap(rhs);
    }

    shadow::Vector normal_to(const shadow::Point & first, const shadow::Point & second, const shadow::Point & third)
    {
        shadow::Vector v(first, second);
        v ^= shadow::Vector(second, third);
        return v / norm_L2(v);
    }

}
