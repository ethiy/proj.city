#pragma once

#include "../../geometry_definitions.h"

#include <vector>
#include <utility>

#include <ostream>

namespace urban
{
    namespace projection
    {
        class FacePrint
        {
        public:
            FacePrint(void);
            FacePrint(const Polygon_with_holes & _border, const Plane_3 & _supporting_plane);
            FacePrint(const FacePrint & other);
            FacePrint(FacePrint && other);
            ~FacePrint(void);

            void swap(FacePrint & other);

            FacePrint & operator=(const FacePrint & other);

            FacePrint & operator=(FacePrint && other);

            Polygon_with_holes get_polygon(void) const noexcept;
            Plane_3 get_plane(void) const noexcept;
            Vector_3 get_normal(void) const noexcept;

            double get_plane_height(const Point_2 &) const;
            double get_height(const Point_2 &) const;
            double area(void) const;

            typedef Polygon_with_holes::Hole_const_iterator Hole_const_iterator;
            Hole_const_iterator holes_begin(void) const;
            Hole_const_iterator holes_end(void) const;
            Polygon outer_boundary(void) const;
            
            /**
            * In our case, two edges are coinciding means that all edges are so,
            * Due to the fact that all edges are supported by the same plane:
            *  - This means that we should just check if the area is null to assert
            *      that the projection is degenarate.
            */
            bool is_degenerate(void) const;
            bool is_perpendicular(void) const;

            bool contains(const Point_2 & point) const;

        private:
            Polygon_with_holes border;
            Plane_3 supporting_plane;

            friend std::ostream & operator<<(std::ostream & os, const FacePrint & facet);
        };
    }

    void swap(projection::FacePrint & lhs, projection::FacePrint & rhs);
    
    /** Compute FacePrint area*/
    double area(const projection::FacePrint & facet);
}
