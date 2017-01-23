#include "face_projection.h"

namespace urban
{
    Plane FaceProjection::get_plane(void) const noexcept
    {
        return original_plane;
    }

    Vector FaceProjection::get_normal(void) const noexcept
    {
        return Vector(original_plane.a(), original_plane.b(), original_plane.c());
    }

    bool FaceProjection::is_inside(const Point_2 & point)
    {
        bool inside(false);
        switch (bounded_side(point))
        {
            case CGAL::ON_BOUNDED_SIDE:
                inside = true; break;
            case CGAL::ON_BOUNDARY:
                inside = true; break;
            case CGAL::ON_UNBOUNDED_SIDE:
                break;
        }
        return inside;
    }

    double FaceProjection::get_height(const Point_2 & point)
    {
        return to_double((original_plane.d() - original_plane.a() * point.x() - original_plane.b() * point.y()) / original_plane.c()) * is_inside(point) ;
    }

    void FaceProjection::set_plane(const Plane & plane) noexcept
    {
        original_plane = plane;
    }
}