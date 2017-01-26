#include "face_projection.h"

#include <algorithm>
#include <iterator>

#include <stdexcept>

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

    double FaceProjection::get_height(const Point_2 & point)
    {
        return to_double((original_plane.d() - original_plane.a() * point.x() - original_plane.b() * point.y()) / original_plane.c()) * contains(point) ;
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

    bool FaceProjection::contains(const FaceProjection & facet)
    {
        bool inside = std::all_of(
            facet.vertices_begin(),
            facet.vertices_end(),
            [this](const Point_2 & point)
            {
                return contains(point);
            }
        );

        bool non_inside = std::none_of(
            facet.vertices_begin(),
            facet.vertices_end(),
            [this](const Point_2 & point)
            {
                return contains(point);
            }
        );
        return inside || (non_inside && contains(CGAL::centroid(facet(0), facet(1), facet(2)))); // prone to problems if centroid is on boundary!!
    }

    void FaceProjection::set_plane(const Plane & plane) noexcept
    {
        original_plane = plane;
    }
}