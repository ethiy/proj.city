#pragma once

#include "../../geometry_definitions.h"

#include <lib3ds/mesh.h>

#include <valarray>
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
            Point(std::valarray<double> const& initializer);
            Point(Point_3 const& point);
            Point(Lib3dsPoint const& point);
            Point(Point const& other);
            Point(Point && other);
            ~Point(void);
            
            std::valarray<double> const& data(void) const noexcept;
            std::valarray<double> & data(void) noexcept;

            double const& x(void) const noexcept;
            double const& y(void) const noexcept;
            double const& z(void) const noexcept;
            double & x(void) noexcept;
            double & y(void) noexcept;
            double & z(void) noexcept;

            void swap(Point & other);
            Point & operator =(Point const& other) noexcept;
            Point & operator =(Point && other) noexcept;

            Point & operator +=(Vector const& translation);

            Bbox bbox(void) const;
        private:
            std::valarray<double> coordinates;

            friend Point operator +(Point const& lhs, Vector const& rhs);
            friend bool operator ==(Point const& lhs, Point const& rhs);

            friend std::ostream & operator <<(std::ostream & os, Point const& point);
        };
        Vector operator -(Point const& lhs, Point const& rhs);
        bool operator !=(Point const& lhs, Point const& rhs);

        void swap(Point & lhs, Point & rhs);

        Vector normal_to(Point const& first, Point const& second, Point const& third);
    }
}
