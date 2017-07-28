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
        Vector::Vector(void)
            : coordinates{{0, 0, 0}} 
        {}
        Vector::Vector(double x, double y, double z)
            : coordinates{{x, y, z}}
        {}
        Vector::Vector(Point const& origin, Point const& target)
            : coordinates{{target.x() - origin.x(), target.y() - origin.y(), target.z() - origin.z()}}
        {}
        Vector::Vector(std::valarray<double> const& initializer)
            : coordinates(initializer)
        {
            if(initializer.size() != 3)
                throw std::logic_error("Cannot create a 3D Vector from this entry! Check the size of the initializer.");
        }
        Vector::Vector(double _coordinates[3])
            : coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}}
        {}
        Vector::Vector(Vector const& other)
            : coordinates(other.coordinates)
        {}
        Vector::Vector(Vector && other)
            : coordinates(std::move(other.coordinates))
        {}
        Vector::~Vector(void)
        {}

        std::valarray<double> const& Vector::data(void) const noexcept
        {
            return coordinates;
        }
        std::valarray<double> & Vector::data(void) noexcept
        {
            return coordinates;
        }

        double const& Vector::x(void) const noexcept
        {
            return coordinates[0];
        }
        double const& Vector::y(void) const noexcept
        {
            return coordinates[1];
        }
        double const& Vector::z(void) const noexcept
        {
            return coordinates[2];
        }
        double & Vector::x(void) noexcept
      {
            return coordinates[0];
        }
        double & Vector::y(void) noexcept
        {
            return coordinates[1];
        }
        double & Vector::z(void) noexcept
        {
            return coordinates[2];
        }

        void Vector::swap(Vector & other)
        {
            using std::swap;
            swap(coordinates, other.coordinates);
        }

        Vector & Vector::operator =(Vector const& other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }

        Vector & Vector::operator =(Vector && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Vector & Vector::operator +=(Vector const& other)
        {
            coordinates += other.coordinates;
            return *this;
        }

        Vector & Vector::operator *=(double const scalar)
        {
            coordinates *= scalar;
            return *this;
        }

        Vector & Vector::operator /=(double const scalar)
        {
            coordinates /= scalar;
            return *this;
        }

        Vector & Vector::operator -=(Vector const& other)
        {
            coordinates -= other.coordinates;
            return *this;
        }

        Vector & Vector::operator ^=(Vector const& other)
        {
            std::valarray<double> coord(coordinates);
            coordinates[0] = coord[1] * other.z() - coord[2] * other.y();
            coordinates[1] = coord[2] * other.x() - coord[0] * other.z();
            coordinates[2] = coord[0] * other.y() - coord[1] * other.x();
            
            return *this;
        }

        std::ostream & operator <<(std::ostream & os, Vector const& vector)
        {
            os << vector.coordinates[0] << " " << vector.coordinates[1] << " " << vector.coordinates[2];
            return os;
        }

        bool operator ==(Vector const& lhs, Vector const& rhs)
        {
            return  (lhs.coordinates == rhs.coordinates).min();
        }


        Vector operator +(Vector const& lhs, Vector const& rhs)
        {
            return Vector(lhs.coordinates + rhs.coordinates);
        }

        Vector operator -(Vector const& lhs, Vector const& rhs)
        {
            return Vector(lhs.coordinates - rhs.coordinates);
        }

        Vector operator *(double const scalar, Vector const& rhs)
        {
            return Vector(scalar * rhs.coordinates);
        }

        Vector operator /(Vector const& lhs, double scalar)
        {
            return Vector(lhs.coordinates / scalar);
        }

        double operator *(Vector const& lhs, Vector const& rhs)
        {
            return (lhs.coordinates * rhs.coordinates).sum();
        }

        Vector operator ^(Vector const& lhs, Vector const& rhs)
        {
            Vector result(lhs);
            return result ^= rhs;
        }

        bool operator !=(Vector const& lhs, Vector const& rhs)
        {
            return !(lhs == rhs);
        }

        void swap(Vector & lhs, Vector & rhs)
        {
            lhs.swap(rhs);
        }

        double norm_L2(Vector & vector)
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
