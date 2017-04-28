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
            Vector(Vector const& other);
            Vector(Vector && other);
            ~Vector(void);

            double x(void) const noexcept;
            double y(void) const noexcept;
            double z(void) const noexcept;

            void swap(Vector & other);

            Vector & operator=(Vector const& other) noexcept;
            Vector & operator=(Vector && other) noexcept;

            Vector & operator+=(Vector const& other);
            Vector & operator*=(double scalar);
            Vector & operator/=(double scalar);
            Vector & operator-=(Vector const& other);
            Vector & operator^=(Vector const& other);

        private:
            std::array<double, 3> coordinates;
            friend std::ostream & operator<<(std::ostream & os, Vector const& vector);
            friend bool operator==(Vector const& lhs, Vector const& rhs);
        };
        Vector & operator+(Vector & lhs, Vector const& rhs);
        Vector & operator-(Vector & lhs, Vector const& rhs);
        double operator*(Vector const& lhs, Vector const& rhs);
        Vector operator*(double scalar, Vector const& rhs);
        Vector operator/(Vector const& lhs, double scalar);
        Vector operator^(Vector const& lhs, Vector const& rhs);
        bool operator!=(Vector const& lhs, Vector const& rhs);
    }

    void swap(shadow::Vector & lhs, shadow::Vector & rhs);
    double norm_L2(shadow::Vector & vector);
    double determinant(const shadow::Vector & first, const shadow::Vector & second, const shadow::Vector & third);
}
