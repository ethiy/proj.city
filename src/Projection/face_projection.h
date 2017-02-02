#pragma once

#include "../geometry_definitions.h"

#include <vector>
#include <utility>

namespace urban
{
    class FaceProjection
    {
    public:
        FaceProjection(void);
        FaceProjection(const Polygon_with_holes &, const Plane &);
        FaceProjection(const FaceProjection &);
        ~FaceProjection(void);

        Polygon_with_holes get_polygon(void) const noexcept;
        Plane get_plane(void) const noexcept;
        Vector get_normal(void) const noexcept;

        double get_plane_height(const Point_2 &);
        double get_height(const Point_2 &);

        typedef Polygon_with_holes::Hole_const_iterator Hole_const_iterator;
        Hole_const_iterator holes_begin(void);
        Hole_const_iterator holes_end(void);
        Polygon outer_boundary(void);
        
        /*! In our case, two edges are coinciding means that all edges are so:
         *  - Due to the fact that all edges are supported by the same plane;
         *  - This means that we should just check if the area is null to assert
         *      that the projection is degenarate.
         */
        bool is_degenerate(void);
        bool is_perpendicular(void) const;

        bool contains(const Point_2 &);

    private:
        Polygon_with_holes projected_polygon;
        Plane supporting_plane;
    };
}
