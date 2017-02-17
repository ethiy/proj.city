#pragma once

//#include "../Point/point.h"

#include <array>

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
            Vector & operator-=(const Vector & other);

        private:
            std::array<double, 3> coordinates;
        };
       
        double determinant(const Vector & first, const Vector & second, const Vector & third);
    }

    void swap(shadow::Vector & lhs, shadow::Vector & rhs);
    
    shadow::Vector & operator+(shadow::Vector & lhs, const shadow::Vector & rhs);
    shadow::Vector & operator-(shadow::Vector & lhs, const shadow::Vector & rhs);
    bool operator==(const shadow::Vector & lhs, const shadow::Vector & rhs);
    bool operator!=(const shadow::Vector & lhs, const shadow::Vector & rhs);
}
