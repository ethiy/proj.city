#include "face_projection.h"

#include "../Algorithms/projection_algorithms.h"

#include <algorithm>
#include <iterator>

#include <stdexcept>

namespace urban
{
    FaceProjection::FaceProjection(void){}
    FaceProjection::FaceProjection(const Polygon_with_holes & _projected_polygon, const Plane & _supporting_plane):projected_polygon(_projected_polygon), supporting_plane(_supporting_plane){}
    FaceProjection::FaceProjection(const FaceProjection & other):projected_polygon(other.projected_polygon), supporting_plane(other.supporting_plane){}
    FaceProjection::~FaceProjection(void){}

    Polygon_with_holes FaceProjection::get_polygon(void) const noexcept
    {
        return projected_polygon;
    }

    Plane FaceProjection::get_plane(void) const noexcept
    {
        return supporting_plane;
    }

    Vector FaceProjection::get_normal(void) const noexcept
    {
        return Vector(supporting_plane.a(), supporting_plane.b(), supporting_plane.c());
    }

    double FaceProjection::get_plane_height(const Point_2 & point)
    {
        if( supporting_plane.c() == 0)
            throw new std::overflow_error("The supporting plane is vertical!");
        return to_double(( -1 * supporting_plane.d() - supporting_plane.a() * point.x() - supporting_plane.b() * point.y()) / supporting_plane.c()) ;
    }


    double FaceProjection::get_height(const Point_2 & point)
    {
        return !is_degenerate() * contains(point) * get_plane_height(point) ;
    }

     FaceProjection::Hole_const_iterator FaceProjection::holes_begin(void)
    {
        return projected_polygon.holes_begin();
    }

     FaceProjection::Hole_const_iterator FaceProjection::holes_end(void)
    {
        return  projected_polygon.holes_end();
    }

    Polygon FaceProjection::outer_boundary(void)
    {
        return projected_polygon.outer_boundary();
    }


    bool FaceProjection::is_degenerate(void)
    {
        return is_perpendicular() || area(*this) == 0;
    }

    bool FaceProjection::is_perpendicular(void) const
    {
        return supporting_plane.c() == 0;
    }

    bool FaceProjection::contains(const Point_2 & point)
    {
        return  projected_polygon.outer_boundary().bounded_side(point) != CGAL::ON_UNBOUNDED_SIDE
                &&
                std::all_of(
                    projected_polygon.holes_begin(),
                    projected_polygon.holes_end(),
                    [point](Polygon hole)
                    {
                        return hole.bounded_side(point) != CGAL::ON_BOUNDED_SIDE;
                    }
                );
    }
}
