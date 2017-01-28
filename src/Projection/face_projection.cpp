#include "face_projection.h"

#include <algorithm>
#include <iterator>

#include <stdexcept>

namespace urban
{
    FaceProjection::FaceProjection(const FaceProjection & other):Polygon(other), original_plane(other.original_plane){}

    Plane FaceProjection::get_plane(void) const noexcept
    {
        return original_plane;
    }

    Vector FaceProjection::get_normal(void) const noexcept
    {
        return Vector(original_plane.a(), original_plane.b(), original_plane.c());
    }

    double FaceProjection::get_height(const Point_2 & point)
    {
        return !is_degenerate() * contains(point) * to_double(( -1 * original_plane.d() - original_plane.a() * point.x() - original_plane.b() * point.y()) / original_plane.c()) ;
    }


    bool FaceProjection::is_degenerate(void) const
    {
        return area() == 0; // In our case, two edges are coinciding means that all edges should be!
    }

    bool FaceProjection::contains(const Point_2 & point)
    {
        bool inside(false);
        switch (bounded_side(point))
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

    void FaceProjection::set_plane(const Plane & plane) noexcept
    {
        original_plane = plane;
    }
}
