#pragma once

#include "../../geometry_definitions.h"
#include "../Point/point.h"

namespace urban
{
    namespace shadow
    {
        static std::size_t get_index(Polyhedron::Halfedge const& halfedge, std::vector<Point> const& points);

        static std::size_t get_index(Polyhedron::Halfedge const& halfedge, std::vector<Point> const& points)
        {
            std::size_t index(0);
            auto point_handle = std::find_if(
                std::begin(points),
                std::end(points),
                [&halfedge](Point const& point)
                {
                    return point == Point(halfedge.vertex()->point());
                }
            );
            if(point_handle != std::end(points))
                index = static_cast<std::size_t>(std::distance(std::begin(points), point_handle));
            else
                throw std::out_of_range("The face contains a non listed point");
            return index;
        }
    }
}
