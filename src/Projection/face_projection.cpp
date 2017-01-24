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

    std::pair<std::vector<size_t>, std::vector<size_t> > FaceProjection::overlap(const FaceProjection & facet)
    {
        std::vector<size_t> starts(facet.size()), lasts(facet.size());

        size_t circulator(0);
        while(circulator < facet.size())
        {
            if(( circulator > 0 && !contains(facet[circulator - 1]) && contains(facet[circulator]) ) || ( circulator == 0 && !contains(facet[facet.size() - 1]) && contains(facet[0]) ))
                starts.push_back(circulator);
            if(( circulator < facet.size()-2 && contains(facet[circulator]) && !contains(facet[circulator + 1]) ) || ( circulator == facet.size()-1 && contains(facet[facet.size() - 1]) && !contains(facet[0]) ))
                lasts.push_back(circulator);
        }

        if(starts.size()!=lasts.size())
            throw new std::out_of_range("There should be the same number of starting and lasting vertices!");

        if( lasts[0] < starts[0] )
            std::rotate(std::begin(lasts), std::next(std::begin(lasts), 1) , std::end(lasts));

        return std::pair<std::vector<size_t>, std::vector<size_t> >(starts, lasts);
    }

    std::vector<Point_2> intersect(const Segment_2 & segment)
    {
        std::vector<Point_2> intersections;
        std::transform(
            edges_begin(),
            edges_end(),
            std::back_inserter(intersections),
            [& segment](const Segment_2 & edge)
            {
                CGAL::cpp11::result_of<Segment_2, Segment_2>::type result = intersection(segment, edge);
            }
        );
        return intersections;
    }

    double FaceProjection::get_height(const Point_2 & point)
    {
        return to_double((original_plane.d() - original_plane.a() * point.x() - original_plane.b() * point.y()) / original_plane.c()) * contains(point) ;
    }

    void FaceProjection::set_plane(const Plane & plane) noexcept
    {
        original_plane = plane;
    }
}