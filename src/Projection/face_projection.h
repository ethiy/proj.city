#pragma once

#include "../geometry_definitions.h"

#include <vector>
#include <utility>

namespace urban
{
    class FaceProjection : Polygon
    {
    public:
        Plane get_plane(void) const noexcept;
        Vector get_normal(void) const noexcept;
        double get_height(const Point_2 &); // problem: degenerate faces

        bool contains(const Point_2 &);
        bool contains(const FaceProjection &);
        std::pair<std::vector<size_t>, std::vector<size_t> > overlap(const FaceProjection &);
        Point_2 intersect(const Segment_2 &);

        void set_plane(const Plane & _plane) noexcept;
    private:
        Plane original_plane;
    };
}