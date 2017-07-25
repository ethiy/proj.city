#pragma once

#include "../../geometry_definitions.h"
#include "../face/face_projection.h"

namespace urban
{
    namespace projection
    {
        Polygon trace(scene::UNode::Facet const& facet, Plane_3 & plane)
        {
            std::vector<Point_2> facet_trace(facet.facet_degree());

            auto h = facet.facet_begin();
            auto iter = std::begin(facet_trace);
            do
            {
                *iter = Point_2(h->vertex()->point().x(), h->vertex()->point().y());
                ++iter;
            }while(++h != facet.facet_begin());

            plane = facet.plane();

            return Polygon(std::begin(facet_trace), std::end(facet_trace));
        }

        FacePrint orthoprint(scene::UNode::Facet const& facet)
        {
            Plane_3 plane;
            Polygon facet_proj = trace(facet, plane);

            if(facet_proj.orientation() == CGAL::CLOCKWISE)
                facet_proj.reverse_orientation();

            return FacePrint(Polygon_with_holes(facet_proj), plane);
        }

        std::vector<FacePrint> orthoprint(scene::UNode const& unode)
        {
            std::vector<FacePrint> prints(unode.facets_size());

            std::transform(
                unode.facets_cbegin(),
                unode.facets_cend(),
                std::begin(prints),
                [](scene::UNode::Facet const& facet)
                {
                    return orthoprint(facet);
                }
            );

            prints.erase(
                std::remove_if(
                    std::begin(prints),
                    std::end(prints),
                    [](FacePrint const& facet)
                    {
                        return facet.is_empty() || facet.is_degenerate();
                    }
                ),
                std::end(prints)
            );

            return prints;
        }

        std::vector<FacePrint> & unpack(std::vector<FacePrint> & facets, Polygon_set polygon_set, Plane_3 const& plane)
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
