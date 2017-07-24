#pragma once

#include "../../geometry_definitions.h"
#include "../face/face_projection.h"

namespace urban
{
    namespace projection
    {
        std::vector<FacePrint> & add(std::vector<FacePrint> & facets, Polygon_set polygon_set, Plane_3 const& plane)
        {
            std::list<Polygon_with_holes> polygons;
            polygon_set.polygons_with_holes(std::back_inserter(polygons));
            
            std::vector<FacePrint> buffer(polygons.size());
            std::transform(
                std::begin(polygons),
                std::end(polygons),
                std::begin(buffer),
                [&plane](Polygon_with_holes const& polygon)
                {
                    return FacePrint(polygon, plane);
                }
            );
            facets.insert(std::end(facets), std::begin(buffer), std::end(buffer));

            return facets;
        }
    }
}
