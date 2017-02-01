#include "face_projection.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <algorithm>
#include <iterator>

#include <stdexcept>

namespace urban
{
    FaceProjection::FaceProjection(void){}
    FaceProjection::FaceProjection(const Polygon & _projected_polygon, const Plane & _supporting_plane):projected_polygon(_projected_polygon), supporting_plane(_supporting_plane){}
    FaceProjection::FaceProjection(const FaceProjection & other):projected_polygon(other.projected_polygon), supporting_plane(other.supporting_plane){}
    FaceProjection::~FaceProjection(void){}

    Plane FaceProjection::get_plane(void) const noexcept
    {
        return supporting_plane;
    }

    Vector FaceProjection::get_normal(void) const noexcept
    {
        return Vector(supporting_plane.a(), supporting_plane.b(), supporting_plane.c());
    }

    double FaceProjection::get_height(const Point_2 & point)
    {
        return !is_degenerate() * contains(point) * to_double(( -1 * supporting_plane.d() - supporting_plane.a() * point.x() - supporting_plane.b() * point.y()) / supporting_plane.c()) ;
    }


    bool FaceProjection::is_degenerate(void) const
    {
        return projected_polygon.area() == 0;
    }

    bool FaceProjection::contains(const Point_2 & point)
    {
        bool inside(false);
        switch (projected_polygon.bounded_side(point))
        {
            case CGAL::ON_BOUNDED_SIDE:
                inside = true; break;
            case CGAL::ON_BOUNDARY:
                break;
            case CGAL::ON_UNBOUNDED_SIDE:
                break;
        }
        return inside;
    }

    void FaceProjection::occlusion(const FaceProjection & other)
    {
        if(CGAL::do_intersect(other.projected_polygon, projected_polygon))
        {
            
        }
    }
}
