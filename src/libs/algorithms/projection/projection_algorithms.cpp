#include "projection_algorithms.h"
#include "../utils/util_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    projection::BrickPrint project(scene::Brick const& brick)
    {
        projection::BrickPrint projection(brick.get_name(), brick.bbox(), brick.get_reference_point(), brick.get_epsg());
        std::vector<projection::FacePrint> projected_facets = project_xy(brick); /** Don't keep perpendicular faces*/
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

    projection::BrickPrint project(scene::Building const& building)
    {
        projection::BrickPrint projection(building.get_name(), building.bbox(), building.pivot(), building.get_epsg());
        projection = building.project_roofs(projection);
        return projection;
    }


    std::vector<projection::FacePrint> project_xy(scene::Brick const& brick)
    {
        std::vector<projection::FacePrint> facets(brick.facets_size());

        std::vector<Point_2> facet_points;
        std::transform(
            brick.facets_cbegin(),
            brick.facets_cend(),
            std::begin(facets),
            [&facet_points](scene::Brick::Facet const& facet)
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

        /** 
         * >> Heuristic sorting in order to have the minimum number of occlusions to deal with;
         */
        //SimpleHeuristic heuristic;
        //std::sort(std::begin(facets), std::end(facets), heuristic);
        return facets;
    }

    projection::BrickPrint & project(projection::BrickPrint & projection, std::vector<scene::Brick> const& bricks)
    {
        projection = std::accumulate(
            std::begin(bricks),
            std::end(bricks),
            projection,
            [](projection::BrickPrint & sum, scene::Brick const& brick)
            {
                return sum + project(brick);
            }
        );
        return projection;
    }

    std::list<projection::FacePrint> occlusion(const projection::FacePrint & lhs, std::list<projection::FacePrint> & rhss)
    {
        std::list<projection::FacePrint> l_result;
        std::list<projection::FacePrint> r_result;
        std::for_each(
            std::begin(rhss),
            std::end(rhss),
            [&r_result, &l_result, &lhs](const projection::FacePrint & rhs)
            {
                /**
                 * >> Checking that faces are not degenerate;
                 *      - Loose degenerate faces on the way.
                 */
                if(!rhs.is_degenerate())
                {
                    Polygon_with_holes first(lhs.get_polygon()), second(rhs.get_polygon());
                    std::list<Polygon_with_holes> superposition(0);
                    /**
                     * >> Checking bounding box overlap before intersection computation;
                     *      - Bounding box checking is faster than Intersection predicate
                     */
                    if(CGAL::do_overlap(first.bbox(), second.bbox()))
                        CGAL::intersection(first, second, std::back_inserter(superposition));

                    /**
                     * >> Checking Intersection
                     */
                    if(!superposition.empty())
                    {
                        Polygon_set first_parts_occluded, second_parts_occluded;
                        /**
                         * >> Assign Intersection to corresponding facet
                         */
                        std::for_each(
                            std::begin(superposition),
                            std::end(superposition),
                            [&lhs, &rhs, &first_parts_occluded, &second_parts_occluded](Polygon_with_holes const& intersection)
                            {
                                Point_2 intersection_point(
                                    CGAL::centroid(
                                        intersection.outer_boundary()[0],
                                        intersection.outer_boundary()[1],
                                        intersection.outer_boundary()[2]
                                        )
                                ); 
                                if(lhs.get_plane_height(intersection_point) > rhs.get_plane_height(intersection_point))
                                    second_parts_occluded.join(intersection);
                                else
                                    first_parts_occluded.join(intersection);
                            }
                        );
                        /**
                         * >> Compute occlusion for each facet
                         */
                        first_parts_occluded.complement();
                        first_parts_occluded.intersection(first);
                        second_parts_occluded.complement();
                        second_parts_occluded.intersection(second);
                        
                        std::list<Polygon_with_holes> _firsts;
                        first_parts_occluded.polygons_with_holes(std::back_inserter(_firsts));
                        std::transform(
                            std::begin(_firsts),
                            std::end(_firsts),
                            std::back_inserter(l_result),
                            [&lhs](Polygon_with_holes part)
                            {
                                return projection::FacePrint(part, lhs.get_plane());
                            }
                        );

                        std::vector<Polygon_with_holes> _seconds;
                        second_parts_occluded.polygons_with_holes(std::back_inserter(_seconds));
                        std::transform(
                            std::begin(_seconds),
                            std::end(_seconds),
                            std::back_inserter(r_result),
                            [&rhs](Polygon_with_holes part)
                            {
                                return projection::FacePrint(part, rhs.get_plane());
                            }
                        );
                    }
                    else
                    {
                        r_result.push_back(rhs);
                        l_result.push_back(lhs);
                    }
                }
            }
        );

        rhss = std::move(r_result);
        return l_result;
    }
}