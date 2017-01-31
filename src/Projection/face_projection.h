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
        FaceProjection(const Polygon &, const Plane &);
        FaceProjection(const FaceProjection &);
        ~FaceProjection(void);

        Plane get_plane(void) const noexcept;
        Vector get_normal(void) const noexcept;
        double get_height(const Point_2 &);
        
        /*! In our case, two edges are coinciding means that all edges are so:
         *  - Due to the fact that all edges are supported by the same plane;
         *  - This means that we should just check if the area is null to assert
         *      that the projection is degenarate.
         */
        bool is_degenerate(void) const;

        bool contains(const Point_2 &);
    private:
        Polygon projected_polygon;
        Plane supporting_plane;
    };
}
