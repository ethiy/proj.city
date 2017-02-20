#pragma once

#include <CGAL/Point_3.h>

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
            template<class Kernel> Point(const CGAL::Point_3<Kernel> & point);
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

            Bbox bbox(void);
        private:
            std::array<double, 3> coordinates;
            friend std::ostream & operator<<(std::ostream & os, const Point & point);
        };

        Vector normal_to(const Point & first, const Point & second, const Point & third);
    }

    void swap(shadow::Point & lhs, shadow::Point & rhs);

    shadow::Point & operator+(shadow::Point & lhs, const shadow::Vector & rhs);
    shadow::Vector operator-(shadow::Point & lhs, const shadow::Point & rhs);
    bool operator==(const shadow::Point & lhs, const shadow::Point & rhs);
    bool operator!=(const shadow::Point & lhs, const shadow::Point & rhs);
}
