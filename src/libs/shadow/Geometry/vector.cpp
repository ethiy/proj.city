#include "vector.h"

#include <algorithm>
#include <numeric>

#include <limits>
#include <stdexcept>

#include <cmath>

namespace urban
{
    namespace shadow
    {
        Vector::Vector(Point const& origin, Point const& target)
            : Coordinates(origin, target)
        {}

        Vector & Vector::operator*=(double const& scalar)
        {
            std::transform(
                std::begin(_coordinates),
                std::end(_coordinates),
                std::begin(_coordinates),
                [scalar](double const& rhs)
                {
                    return scalar * rhs;
                }
            );
            return *this;
        }

        Vector & Vector::operator/=(double const& scalar)
        {
            if(std::abs(scalar) < std::numeric_limits<double>::epsilon())
                throw std::overflow_error("Division by zero");
            std::transform(
                std::begin(_coordinates),
                std::end(_coordinates),
                std::begin(_coordinates),
                [scalar](double const& rhs)
                {
                    return rhs / scalar;
                }
            );
            return *this;
        }

        Vector & Vector::operator^=(Vector const& other)
        {
            std::array<double, 3> coord(_coordinates);
            _coordinates[0] = coord.at(1) * other.z() - coord.at(2) * other.y();
            _coordinates[1] = coord.at(2) * other.x() - coord.at(0) * other.z();
            _coordinates[2] = coord.at(0) * other.y() - coord.at(1) * other.x();
            
            return *this;
        }

        double operator*(Vector const& lhs, Vector const& rhs)
        {
            return std::inner_product(
                std::begin(lhs),
                std::end(lhs),
                std::begin(rhs),
                0
            );
        }

        Vector operator*(double const& scalar, Vector const& rhs)
        {
            Vector result;
            std::transform(
                std::begin(rhs),
                std::end(rhs),
                std::begin(result),
                [scalar](double const& _rhs)
                {
                    return scalar * _rhs;
                }
            );
            return result;
        }

        Vector operator/(Vector const& lhs, double const& scalar)
        {
            if(std::abs(scalar) < std::numeric_limits<double>::epsilon())
                throw std::overflow_error("Division by zero");

            Vector result;
            std::transform(
                std::begin(lhs),
                std::end(lhs),
                std::begin(result),
                [scalar](double const& _lhs)
                {
                    return _lhs / scalar;
                }
            );
            return result;
        }

        Vector operator^(Vector const& lhs, Vector const& rhs)
        {
            Vector result;
            result.x() = lhs.y() * rhs.z() - lhs.z() * rhs.y();
            result.y() = lhs.z() * rhs.x() - lhs.x() * rhs.z();
            result.z() = lhs.x() * rhs.y() - lhs.y() * rhs.x();
            
            return result;
        }
    }
    double norm_L2(shadow::Vector const& vector)
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
