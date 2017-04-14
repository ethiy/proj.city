#pragma once

#include "../../geometry_definitions.h"

#include <lib3ds/mesh.h>

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Vector;
        class Bbox;

        /** 
         * @ingroup shadow_group
         * @brief Point class representing a 3D Point.
         * 
         * Shadow Point is member class of Shadow Mesh:
         *  - It stores a 3d Point coordinates.
         */
        class Point
        {
        public:
            Point(void);
            Point(double x, double y, double z);
            Point(double _coordinates[3]);
            Point(Point_3 const& point);
            Point(Lib3dsPoint const& point);
            Point(Point const& other);
            Point(Point && other);
            ~Point(void);

            double x(void) const noexcept;
            double y(void) const noexcept;
            double z(void) const noexcept;

            void swap(Point & other);
            Point & operator=(Point const& other) noexcept;
            Point & operator=(Point && other) noexcept;

            Point & operator+=(Vector const& translation);

            Bbox bbox(void) const;
        private:
            std::array<double, 3> coordinates;
            friend std::ostream & operator<<(std::ostream & os, Point const& point);
        };
        Vector operator-(Point const& lhs, Point const& rhs);
        Point & operator+(Point & lhs, Vector const& rhs);
        bool operator==(Point const& lhs, Point const& rhs);
        bool operator!=(Point const& lhs, Point const& rhs);
    }
    void swap(shadow::Point & lhs, shadow::Point & rhs);
    shadow::Vector normal_to(shadow::Point const& first, shadow::Point const& second, shadow::Point const& third);
}
