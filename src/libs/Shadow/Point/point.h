#pragma once

#include "../Vector/vector.h"

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Point
        {
        public:
            Point(void);
            Point(double x, double y, double z);
            Point(double _coordinates[3]);
            Point(const Point & other);
            Point(Point && other);
            ~Point(void);

            double x(void);
            double y(void);
            double z(void);

            void swap(Point & other);

            Point & operator=(const Point & other) noexcept;
            Point & operator=(Point && other) noexcept;

        private:
            std::array<double, 3> coordinates;
        };
    }

    void swap(shadow::Point & lhs, shadow::Point & rhs);

    // shadow::Point & operator+(const shadow::Point & lhs, const shadow::Vector & rhs);
    // shadow::Vector & operator-(const shadow::Point & lhs, const shadow::Point & rhs);
    bool operator==(const shadow::Point & lhs, const shadow::Point & rhs);
    bool operator!=(const shadow::Point & lhs, const shadow::Point & rhs);
    std::ostream & operator<<(std::ostream & os, shadow::Point & point);
}
