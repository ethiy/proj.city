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
        Vector::Vector(void)
        {}
        Vector::Vector(double x, double y, double z)
            : Coordinates(x, y, z)
        {}
        Vector::Vector(const double initializer[3])
            : Coordinates(initializer)
        {}
        Vector::Vector(std::valarray<double> const& initializer)
            : Coordinates(initializer)
        {}
        Vector::Vector(Vector const& other)
            : Coordinates(other)
        {}
        Vector::Vector(Vector && other)
            : Coordinates(other)
        {}
        Vector::Vector(Point const& origin, Point const& target)
            : Coordinates(origin, target)
        {}
        Vector::~Vector(void)
        {}

        void Vector::swap(Vector & other)
        {
            Coordinates::swap(other);
        }
        Vector & Vector::operator =(Vector const& other)
        {
            Coordinates::operator =(other);
            return *this;
        }
        Vector & Vector::operator =(Vector && other)
        {
            Coordinates::operator =(other);
            return *this;
        }

        Vector & Vector::operator *=(double const& scalar)
        {
            Coordinates::_coordinates *= scalar;
            return *this;
        }
        Vector & Vector::operator /=(double const& scalar)
        {
            Coordinates::_coordinates /= scalar;
            return *this;
        }
        Vector & Vector::operator ^=(Vector const& other)
        {
            std::valarray<double> coord(_coordinates);

            Coordinates::_coordinates[0] = coord[1] * other.z() - coord[2] * other.y();
            Coordinates::_coordinates[1] = coord[2] * other.x() - coord[0] * other.z();
            Coordinates::_coordinates[2] = coord[0] * other.y() - coord[1] * other.x();
            
            return *this;
        }
        double operator *(Vector const& lhs, Vector const& rhs)
        {
            return (lhs._coordinates * rhs._coordinates).sum();
        }
        Vector operator *(double const& scalar, Vector const& rhs)
        {
            Vector result(rhs);
            result *= scalar;
            return result;
        }
        Vector operator /(Vector const& lhs, double const& scalar)
        {
            Vector result(lhs);
            result /= scalar;
            return result;
        }
        Vector operator ^(Vector const& lhs, Vector const& rhs)
        {
            Vector result;
            
            result.x() = lhs.y() * rhs.z() - lhs.z() * rhs.y();
            result.y() = lhs.z() * rhs.x() - lhs.x() * rhs.z();
            result.z() = lhs.x() * rhs.y() - lhs.y() * rhs.x();
            
            return result;
        }

        void swap(Vector & lhs, Vector & rhs)
        {
            lhs.swap(rhs);
        }

        double norm_L2(Vector const& vector)
        {
            return std::sqrt(vector * vector);
        }

        double determinant(Vector const& first, Vector const& second, Vector const& third)
        {
            return  first.x() * second.y() * third.z() - first.x() * second.z() * third.y() -
                    first.y() * second.x() * third.z() + first.y() * second.z() * third.x() +
                    first.z() * second.x() * third.y() - first.z() * second.y() * third.x();
        }
    }
}
