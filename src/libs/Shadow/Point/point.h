#pragma once

// #include "../Vector/vector.h"

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Vector;

        class Point
        {
        public:
            Point(void);
            Point(double x, double y, double z);
            Point(double _coordinates[3]);
            Point(const Point & other);
            Point(Point && other);
            ~Point(void);

            double x(void) const noexcept;
            double y(void) const noexcept;
            double z(void) const noexcept;

            void swap(Point & other);

            Point & operator=(const Point & other) noexcept;
            Point & operator=(Point && other) noexcept;

            Point & operator+=(const Vector & translation);

        private:
            std::array<double, 3> coordinates;
        };
    }

    void swap(shadow::Point & lhs, shadow::Point & rhs);

    shadow::Point & operator+(shadow::Point & lhs, const shadow::Vector & rhs);
    shadow::Vector operator-(shadow::Point & lhs, const shadow::Point & rhs);
    bool operator==(const shadow::Point & lhs, const shadow::Point & rhs);
    bool operator!=(const shadow::Point & lhs, const shadow::Point & rhs);
    std::ostream & operator<<(std::ostream & os, shadow::Point & point);
}
