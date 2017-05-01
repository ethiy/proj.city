#include "vector.h"

#include <algorithm>

#include <cmath>
#include <limits>
#include <stdexcept>

#include "../Point/point.h"

namespace urban
{
    namespace shadow
    {
        Vector::Vector(void): coordinates{{0, 0, 0}} {}
        Vector::Vector(double x, double y, double z): coordinates{{x, y, z}} {}
        Vector::Vector(Point const& origin, Point const& target): coordinates{{target.x() - origin.x(), target.y() - origin.y(), target.z() - origin.z()}} {}
        Vector::Vector(double _coordinates[3]): coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}} {}
        Vector::Vector(Vector const& other): coordinates(other.coordinates) {}
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

        Vector & Vector::operator=(Vector const& other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }

        Vector & Vector::operator=(Vector && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Vector & Vector::operator+=(Vector const& other)
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

        Vector & Vector::operator*=(double scalar)
        {
            std::transform(
                std::begin(coordinates),
                std::end(coordinates),
                std::begin(coordinates),
                [scalar](const double rhs)
                {
                    return scalar * rhs;
                }
            );
            return *this;
        }

        Vector & Vector::operator/=(double scalar)
        {
            if(std::abs(scalar) < std::numeric_limits<double>::epsilon())
                throw std::overflow_error("Division by zero");
            std::transform(
                std::begin(coordinates),
                std::end(coordinates),
                std::begin(coordinates),
                [scalar](const double rhs)
                {
                    return rhs / scalar;
                }
            );
            return *this;
        }

        Vector & Vector::operator-=(Vector const& other)
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

        Vector & Vector::operator^=(Vector const& other)
        {
            std::array<double, 3> coord(coordinates);
            coordinates[0] = coord.at(1) * other.z() - coord.at(2) * other.y();
            coordinates[1] = coord.at(2) * other.x() - coord.at(0) * other.z();
            coordinates[2] = coord.at(0) * other.y() - coord.at(1) * other.x();
            
            return *this;
        }

        std::ostream & operator<<(std::ostream & os, Vector const& vector)
        {
            os << vector.coordinates.at(0) << " " << vector.coordinates.at(1) << " " << vector.coordinates.at(2);
            return os;
        }

        bool operator==(Vector const& lhs, Vector const& rhs)
        {
            return  std::abs(lhs.x() - rhs.x()) < std::numeric_limits<double>::epsilon()
                    &&
                    std::abs(lhs.y() - rhs.y()) < std::numeric_limits<double>::epsilon()
                    &&
                    std::abs(lhs.z() - rhs.z()) < std::numeric_limits<double>::epsilon();
        }


        Vector & operator+(Vector & lhs, Vector const& rhs)
        {
            return lhs += rhs;
        }

        Vector & operator-(Vector & lhs, Vector const& rhs)
        {
            return lhs -= rhs;
        }

        Vector operator*(double scalar, Vector const& rhs)
        {
            Vector copy(rhs);
            return copy *= scalar;
        }

        Vector operator/(Vector const& lhs, double scalar)
        {
            Vector copy(lhs);
            return copy /= scalar;
        }

        double operator*(Vector const& lhs, Vector const& rhs)
        {
            return  lhs.x() * rhs.x()
                    +
                    lhs.y() * rhs.y()
                    +
                    lhs.z() * rhs.z();
        }

        Vector operator^(Vector const& lhs, Vector const& rhs)
        {
            Vector copy(lhs);
            return copy ^= rhs;;
        }

        bool operator!=(Vector const& lhs, Vector const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(shadow::Vector & lhs, shadow::Vector & rhs)
    {
        lhs.swap(rhs);
    }

    double norm_L2(shadow::Vector & vector)
    {
        return std::sqrt(vector * vector);
    }

    double determinant(shadow::Vector const& first, shadow::Vector const& second, shadow::Vector const& third)
    {
        return first.x() * second.y() * third.z() - first.x() * second.z() * third.y() -
               first.y() * second.x() * third.z() + first.y() * second.z() * third.x() +
               first.z() * second.x() * third.y() - first.z() * second.y() * third.x();
    }

}
