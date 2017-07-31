#pragma once

#include <geometry_definitions.h>
#include <shadow/point.h>

namespace urban
{
    namespace shadow
    {
        std::size_t get_index(Polyhedron::Halfedge const& halfedge, std::vector<Point> const& points);
    }
}
