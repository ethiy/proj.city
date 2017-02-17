#include "vector.h"

#include <algorithm>

namespace urban
{
    namespace shadow
    {
        Vector::Vector(void): coordinates{{0, 0, 0}} {}
        Vector::Vector(double x, double y, double z): coordinates{{x, y, z}} {}
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

    bool operator==(const shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        shadow::Vector l_copy(lhs);
        return (l_copy - rhs) == shadow::Vector();
    }

    bool operator!=(const shadow::Vector & lhs, const shadow::Vector & rhs)
    {
        return !(lhs == rhs);
    }

}
