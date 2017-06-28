#pragma once

#include <valarray>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Point;

        class Vector
        {
        public:
            Vector(void);
            Vector(const Point & origin, const Point & target);
            Vector(double x, double y, double z);
            Vector(double coordinates[3]);
            Vector(std::valarray<double> const& initializer);
            Vector(Vector const& other);
            Vector(Vector && other);
            ~Vector(void);

            std::valarray<double> const& data(void) const noexcept;
            std::valarray<double> & data(void) noexcept;

            double const& x(void) const noexcept;
            double const& y(void) const noexcept;
            double const& z(void) const noexcept;
            double & x(void) noexcept;
            double & y(void) noexcept;
            double & z(void) noexcept;

            void swap(Vector & other);

            Vector & operator =(Vector const& other) noexcept;
            Vector & operator =(Vector && other) noexcept;

            Vector & operator +=(Vector const& other);
            Vector & operator *=(double const scalar);
            Vector & operator /=(double const scalar);
            Vector & operator -=(Vector const& other);
            Vector & operator ^=(Vector const& other);

        private:
            std::valarray<double> coordinates;

            friend bool operator ==(Vector const& lhs, Vector const& rhs);
            friend Vector operator +(Vector const& lhs, Vector const& rhs);
            friend Vector operator -(Vector const& lhs, Vector const& rhs);
            friend double operator *(Vector const& lhs, Vector const& rhs);
            friend Vector operator *(double const scalar, Vector const& rhs);
            friend Vector operator /(Vector const& lhs, double const scalar);

            friend std::ostream & operator <<(std::ostream & os, Vector const& vector);
        };
        Vector operator ^(Vector const& lhs, Vector const& rhs);
        bool operator !=(Vector const& lhs, Vector const& rhs);

        void swap(Vector & lhs, Vector & rhs);
    }
    double norm_L2(shadow::Vector & vector);
    double determinant(const shadow::Vector & first, const shadow::Vector & second, const shadow::Vector & third);
}
