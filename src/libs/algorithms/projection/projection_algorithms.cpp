#include "projection_algorithms.h"
#include "../utils/util_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    projection::BrickPrint project(scene::UNode const& unode)
    {
        projection::BrickPrint projection(unode.get_name(), unode.bbox(), unode.get_reference_point(), unode.get_epsg());
        std::vector<projection::FacePrint> projected_facets = project_xy(unode); /** Don't keep perpendicular faces*/
        std::for_each(
            std::begin(projected_facets),
            std::end(projected_facets),
            [&projection](projection::FacePrint const& facet)
            {
                projection.insert(facet);
            }
        );
        return projection;
    }

    projection::BrickPrint project(scene::Scene const& scene)
    {
        // projection::BrickPrint projection(scene.get_name(), scene.bbox(), scene.pivot(), scene.get_epsg());
        // projection = scene.project_roofs(projection);
        return projection::BrickPrint();
    }


    std::vector<projection::FacePrint> project_xy(scene::UNode const& unode)
    {
        std::vector<projection::FacePrint> facets(unode.facets_size());

        std::vector<Point_2> facet_points;
        std::transform(
            unode.facets_cbegin(),
            unode.facets_cend(),
            std::begin(facets),
            [&facet_points](scene::UNode::Facet const& facet)
            {
                projection::FacePrint projected_facet;
                /**
                 * >> Copying 3D points to 2D Point vector
                 */
                facet_points = std::vector<Point_2>(facet.facet_degree());

                /** Start with the first point*/
                auto halfedge = facet.halfedge();
                Point_3 vertex(halfedge->vertex()->point());
                facet_points.at(0) = Point_2(vertex.x(), vertex.y());
                std::transform(
                    std::next(facet.facet_begin()),
                    std::next(facet.facet_begin(), static_cast<long>(facet.facet_degree())),
                    std::next(std::begin(facet_points)),
                    [&vertex](Polyhedron::Halfedge const& h)
                    {
                        return Point_2(h.vertex()->point().x(), h.vertex()->point().y());
                    }
                );

                /**
                 * >> Plane equation
                 */
                Plane_3 plane_equation(
                    halfedge->vertex()->point(),
                    halfedge->next()->vertex()->point(),
                    halfedge->next()->next()->vertex()->point()
                );

                /**
                 * Only non (Numerically) perpendicular faces are kept;
                 */
                if(plane_equation.c() != 0)
                {                    
                    Polygon outer_boundary(Polygon(std::begin(facet_points), std::end(facet_points)));
                    
                    if(outer_boundary.orientation() == CGAL::CLOCKWISE)
                        outer_boundary.reverse_orientation();
                    
                    projected_facet = projection::FacePrint(Polygon_with_holes(outer_boundary), plane_equation);
                }

                return projected_facet;
            }
        );

        /**
         * >> Filter null elements
         */
        facets.erase(
            std::remove_if(
                std::begin(facets),
                std::end(facets),
                [](const projection::FacePrint & facet)
                {
                    return facet.is_empty() || facet.is_perpendicular();
                }
            ),
            std::end(facets)
        );
        return facets;
    }

    projection::BrickPrint & project(projection::BrickPrint & projection, std::vector<scene::UNode> const& unodes)
    {
        projection = std::accumulate(
            std::begin(unodes),
            std::end(unodes),
            projection,
            [](projection::BrickPrint & sum, scene::UNode const& unode)
            {
                return sum + project(unode);
            }
        );
        return projection;
    }
}
