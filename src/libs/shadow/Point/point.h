#pragma once

#include "../../geometry_definitions.h"

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Vector;
        class Bbox;

        class Point
        {
        public:
            Point(void);
            Point(double x, double y, double z);
            Point(double _coordinates[3]);
            Point(const Point_3 & point);
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

            Bbox bbox(void) const;
        private:
            std::array<double, 3> coordinates;
            friend std::ostream & operator<<(std::ostream & os, const Point & point);
        };
        Vector operator-(const Point & lhs, const Point & rhs);
        Point & operator+(Point & lhs, const Vector & rhs);
        bool operator==(const Point & lhs, const Point & rhs);
        bool operator!=(const Point & lhs, const Point & rhs);
    }
    void swap(shadow::Point & lhs, shadow::Point & rhs);
    shadow::Vector normal_to(const shadow::Point & first, const shadow::Point & second, const shadow::Point & third);
}
