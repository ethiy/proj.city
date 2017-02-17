#include "vector.h"

#include <algorithm>

#include <cmath>
#include <limits>

#include "../Point/point.h"

namespace urban
{
    namespace shadow
    {
        Vector::Vector(void): coordinates{{0, 0, 0}} {}
        Vector::Vector(double x, double y, double z): coordinates{{x, y, z}} {}
        Vector::Vector(const Point & origin, const Point & target): coordinates{{target.x() - origin.x(), target.y() - origin.y(), target.z() - origin.z()}} {}
        Vector::Vector(double _coordinates[3]): coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}} {}
        Vector::Vector(const Vector & other): coordinates(other.coordinates) {}
        Vector::Vector(Vector && other): coordinates(std::move(other.coordinates)) {}
        Vector::~Vector(void){}

        double Vector::x(void) const noexcept
        {
            return coordinates.at(0);
        }
        double Vector::y(void) const noexcept
        {
            return coordinates.at(1);
        }
        double Vector::z(void) const noexcept
        {
            return coordinates.at(2);
        }

        void Vector::swap(Vector & other)
        {
            using std::swap;
            swap(coordinates, other.coordinates);
        }

        Vector & Vector::operator=(const Vector & other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }

        Vector & Vector::operator=(Vector && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Vector & Vector::operator+=(const Vector & other)
        {
            std::transform(
                std::begin(coordinates),
                std::end(coordinates),
                std::begin(other.coordinates),
                std::begin(coordinates),
                [](const double lhs, const double rhs)
                {
                    return lhs + rhs;
                }
            );
            return *this;
        }

        Vector & Vector::operator-=(const Vector & other)
        {
            std::transform(
                std::begin(coordinates),
                std::end(coordinates),
                std::begin(other.coordinates),
                std::begin(coordinates),
                [](const double lhs, const double rhs)
                {
                    return lhs - rhs;
                }
            );
            return *this;
        }

        Vector & Vector::operator^=(const Vector & other)
        {
            std::array<double, 3> coord(coordinates);
            coordinates[0] = coord.at(1) * other.z() - coord.at(2) * other.y();
            coordinates[1] = coord.at(2) * other.x() - coord.at(0) * other.z();
            coordinates[2] = coord.at(0) * other.y() - coord.at(1) * other.x();
            
            return *this;
        }
        double determinant(const Vector & first, const Vector & second, const Vector & third)
        {
            return first.x() * (second.y() * third.z() - second.z() - third.y()) -
                   first.y() * (second.x() * third.z() - second.z() - third.x()) +
                   first.z() * (second.x() * third.y() - second.y() - third.x());
        }
    }

    void swap(shadow::Vector & lhs, shadow::Vector & rhs)
    {
        lhs.swap(rhs);
    }

    shadow::Vector & operator+(shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        return lhs += rhs;
    }

    shadow::Vector & operator-(shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        return lhs-=rhs;
    }

    double operator*(const shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        return lhs.x() * rhs.x()
               *
               lhs.y() * rhs.y()
               *
               lhs.z() * rhs.z();
    }

    shadow::Vector & operator^(shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        return lhs ^= rhs;
    }

    bool operator==(const shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        shadow::Vector diff(lhs);
        diff -= rhs;
        return std::abs(diff.x()) < std::numeric_limits<double>::epsilon() &&
               std::abs(diff.y()) < std::numeric_limits<double>::epsilon() &&
               std::abs(diff.z()) < std::numeric_limits<double>::epsilon();
    }

    bool operator!=(const shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        return !(lhs == rhs);
    }

}
