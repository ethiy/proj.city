#include "point.h"

#include "../Vector/vector.h"

namespace urban
{
    namespace shadow
    {
        Point::Point(void): coordinates{{0, 0, 0}} {}
        Point::Point(double x, double y, double z): coordinates{{x, y, z}} {}
        Point::Point(double _coordinates[3]): coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}} {}
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

        Vector normal(const Point & first, const Point & second, const Point & third)
        {
            return (second - first)^(third - second);
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

    std::ostream & operator<<(std::ostream & os, shadow::Point & point)
    {
        os << point.x() << " " << point.y() << " " << point.z();
        return os;
    }

}
