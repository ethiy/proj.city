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
            Point(Lib3dsPoint const& point);
            Point(Point_3 const& point);

            Bbox bbox(void) const;
        };
    }

    shadow::Vector normal_to(shadow::Point const& first, shadow::Point const& second, shadow::Point const& third);
}
