#pragma once

#include "cartesian_coordinates.h"

#include "../Bbox/bbox.h"

namespace urban
{
    namespace shadow
    {
        class Bbox;
        class Vector;

        /** 
         * @ingroup shadow_group
         * @brief Point class representing a 3D Point.
         * 
         * Shadow Point is member class of Shadow Mesh:
         *  - It stores a 3d Point coordinates.
         */
        class Point : public Coordinates
        {
        public:
            using Coordinates::Coordinates;
            Point(void);
            Point(double x, double y, double z);
            Point(const double initializer[3]);
            Point(std::valarray<double> const& initializer);
            Point(Point const& other);
            Point(Point && other);
            Point(Lib3dsPoint const& point);
            Point(Point_3 const& point);
            ~Point(void);

            Bbox bbox(void) const;

            void swap(Point & other);
            Point & operator =(Point const& other) noexcept;
            Point & operator =(Point && other) noexcept;
        protected:
            friend bool operator ==(Point const& lhs, Point const& rhs);
            friend Vector operator -(Point const& lhs, Point const& rhs);

            friend std::ostream & operator <<(std::ostream & os, Point const& rhs);
        };
        void swap(Point & lhs, Point & rhs);

        bool operator !=(Point const& lhs, Point const& rhs);

        Vector normal_to(Point const& first, Point const& second, Point const& third);
    }
}
