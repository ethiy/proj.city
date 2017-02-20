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
        template<class Kernel> Point::Point(const CGAL::Point_3<Kernel> & point):coordinates{{CGAL::to_double(point.x()), CGAL::to_double(point.y()), CGAL::to_double(point.z())}} {}
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

        Bbox Point::bbox(void)
        {
            return Bbox(coordinates);
        }

        Vector normal(const Point & first, const Point & second, const Point & third)
        {
            Vector v(first, second);
            return v ^ Vector(second, third);
        }
        std::ostream & operator<<(std::ostream & os, const Point & point)
        {
            std::copy(std::begin(point.coordinates), std::end(point.coordinates), std::ostream_iterator<double>(os, " "));
            return os;
        }

    }

    void swap(shadow::Point & lhs, shadow::Point & rhs)
    {
        lhs.swap(rhs);
    }

    shadow::Point & operator+(shadow::Point & lhs, const shadow::Vector & rhs)
    {
        return lhs += rhs;
    }

    shadow::Vector operator-(const shadow::Point & lhs, const shadow::Point & rhs)
    {
        return shadow::Vector(rhs, lhs);
    }

    bool operator==(const shadow::Point & lhs, const shadow::Point & rhs)
    {
        return (rhs - lhs) == shadow::Vector();
    }
    
    bool operator!=(const shadow::Point & lhs, const shadow::Point & rhs)
    {
        return !(rhs == lhs);
    }
}
