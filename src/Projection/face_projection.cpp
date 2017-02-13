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
    FaceProjection::FaceProjection(FaceProjection && other):projected_polygon(std::move(other.projected_polygon)), supporting_plane(std::move(other.supporting_plane)){}
    FaceProjection::~FaceProjection(void){}

    void FaceProjection::swap(FaceProjection & other)
    {
        using std::swap;
        swap(projected_polygon, other.projected_polygon);
        swap(supporting_plane, other.supporting_plane);
    }

    FaceProjection & FaceProjection::operator=(const FaceProjection & other)
    {
        projected_polygon = other.projected_polygon;
        supporting_plane = other.supporting_plane;
        return *this;
    }

    FaceProjection & FaceProjection::operator=(FaceProjection && other)
    {
        projected_polygon = std::move(other.projected_polygon);
        supporting_plane = std::move(other.supporting_plane);
        return *this;
    }

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

    double FaceProjection::get_plane_height(const Point_2 & point) const
    {
        if( supporting_plane.c() == 0)
            throw std::overflow_error("The supporting plane is vertical!");
        return ( -1 * supporting_plane.d() - supporting_plane.a() * point.x() - supporting_plane.b() * point.y()) / supporting_plane.c() ;
    }


    double FaceProjection::get_height(const Point_2 & point) const
    {
        return !is_degenerate() * contains(point) * get_plane_height(point) ;
    }

    double FaceProjection::area(void) const
    {
        return std::accumulate(
                    projected_polygon.holes_begin(),
                    projected_polygon.holes_end(),
                    projected_polygon.outer_boundary().area(),
                    [](double & holes_area, const Polygon & hole)
                    {
                        return holes_area - hole.area();
                    }
                );
    }

     FaceProjection::Hole_const_iterator FaceProjection::holes_begin(void) const
    {
        return projected_polygon.holes_begin();
    }

     FaceProjection::Hole_const_iterator FaceProjection::holes_end(void) const
    {
        return  projected_polygon.holes_end();
    }

    Polygon FaceProjection::outer_boundary(void) const
    {
        return projected_polygon.outer_boundary();
    }


    bool FaceProjection::is_degenerate(void) const
    {
        return is_perpendicular() || ( holes_begin() != holes_end() && area() == 0); /* If it has no holes no need to check surface */
    }

    bool FaceProjection::is_perpendicular(void) const
    {
        return supporting_plane.c() == 0;
    }

    bool FaceProjection::contains(const Point_2 & point) const
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

    void swap(FaceProjection & lhs, FaceProjection & rhs)
    {
        lhs.swap(rhs);
    }
}
