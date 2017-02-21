#pragma once

#include <array>
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
            Vector(const Vector & other);
            Vector(Vector && other);
            ~Vector(void);

            double x(void) const noexcept;
            double y(void) const noexcept;
            double z(void) const noexcept;

            void swap(Vector & other);

            Vector & operator=(const Vector & other) noexcept;
            Vector & operator=(Vector && other) noexcept;

            Vector & operator+=(const Vector & other);
            Vector & operator*=(double scalar);
            Vector & operator/=(double scalar);
            Vector & operator-=(const Vector & other);
            Vector & operator^=(const Vector & other);

        private:
            std::array<double, 3> coordinates;
            friend std::ostream & operator<<(std::ostream & os, Vector & vector);
        };
        Vector & operator+(Vector & lhs, const Vector & rhs);
        Vector & operator-(Vector & lhs, const Vector & rhs);
        double operator*(const Vector & lhs, const Vector & rhs);
        Vector operator*(double scalar, const Vector & rhs);
        Vector operator/(const Vector & lhs, double scalar);
        Vector operator^(const Vector & lhs, const Vector & rhs);
        bool operator==(const Vector & lhs, const Vector & rhs);
        bool operator!=(const Vector & lhs, const Vector & rhs);
    }

    void swap(shadow::Vector & lhs, shadow::Vector & rhs);
    double norm_L2(shadow::Vector & vector);
    double determinant(const shadow::Vector & first, const shadow::Vector & second, const shadow::Vector & third);
}
