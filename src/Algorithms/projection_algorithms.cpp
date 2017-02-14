#include "projection_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <vector>
#include <iterator>
#include <algorithm>

#include <cmath>

#include <stdexcept>

namespace urban
{
    Affine_transformation rotation_transform(const std::map<double, Vector> & _rotations)
    {
        std::vector<Affine_transformation> rotations;
        double norm(0);
        Vector u(CGAL::NULL_VECTOR);
        std::transform(
            std::begin(_rotations),
            std::end(_rotations),
            std::begin(rotations),
            [norm, u](const std::pair<double, Vector> & angle_axis) mutable
            {
                norm = std::sqrt(to_double(angle_axis.second * angle_axis.second));
                u = angle_axis.second / norm;
                return Affine_transformation(
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.x()), 2.), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.y()) - std::sin(angle_axis.first) * to_double(u.z()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.z()) + std::sin(angle_axis.first) * to_double(u.y()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.y()) + std::sin(angle_axis.first) * to_double(u.z()), 
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.y()), 2.), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.y()) * to_double(u.z()) - std::sin(angle_axis.first) * to_double(u.x()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.z()) - std::sin(angle_axis.first) * to_double(u.y()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.y()) * to_double(u.z()) + std::sin(angle_axis.first) * to_double(u.x()), 
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.z()), 2.)
                    );
            }
        );

        return std::accumulate(
            std::begin(rotations),
            std::end(rotations),
            Affine_transformation(CGAL::IDENTITY),
            [](Affine_transformation & result, const Affine_transformation & rotation)
            {
                return result * rotation;
            }
        );
    }

    bool check_collinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last)
    {
        if(std::distance(first, last) < 3)
            throw std::out_of_range("There are less than 3 points between the two iterators!");
        /**
         * It suffices to check the first three points;
         */
        return CGAL::collinear(*first, *(first + 1), *(first + 2));
    }

    void extrem_points(std::vector<Point_2> & points)
    {
        Point_2 A(points[0]), B(points[1]);
        double AB(.0), AC(.0), BC(.0);
        std::for_each(
            std::next(std::begin(points), 2),
            std::end(points),
            [&](Point_2 C)
            {
                AB = to_double(CGAL::squared_distance(A, B));
                AC = to_double(CGAL::squared_distance(A, C));
                BC = to_double(CGAL::squared_distance(B, C));
                if(AC/AB > 1 || BC/AB > 1)
                {
                    if(AB/AC > 1 || BC/AC > 1)
                        A = C;
                    else
                        B = C;
                }
            }
        );
        points.clear();
        points.push_back(A);
        points.push_back(B);
    }

    double area(const projection::FacePrint & facet)
    {
        return facet.area();
    }


    projection::BrickPrint project(const Brick & brick)
    {
        projection::BrickPrint projection(brick.get_name(), brick.bbox());
        std::vector<projection::FacePrint> projected_facets = project_xy(brick); /** Don't keep perpendicular faces*/
        size_t it(0);
        std::for_each(
            std::begin(projected_facets),
            std::end(projected_facets),
            [&projection, &it](projection::FacePrint facet)
            {
                std::cout << ">> Face projection number: " << it++ << std::endl;
                projection.push_facet(facet);
            }
        );
        return projection;
    }


    std::vector<projection::FacePrint> project_xy(const Brick & brick)
    {
        std::vector<projection::FacePrint> facets(brick.facets_number());

        std::vector<Point_2> facet_points;
        std::transform(
            brick.facets_cbegin(),
            brick.facets_cend(),
            std::begin(facets),
            [&facet_points](const Facet & facet)
            {
                projection::FacePrint projected_facet;
                /**
                 * >> Copying 3D points to 2D Point vector
                 */
                facet_points.clear();
                facet_points.resize(facet.facet_degree());

                /** Start with the first point*/
                auto halfedge = facet.halfedge();
                Point vertex(halfedge->vertex()->point());
                facet_points[0] = Point_2(vertex.x(), vertex.y());
                std::transform(
                    std::next(facet.facet_begin(), 1),
                    std::next(facet.facet_begin(), static_cast<long>(facet.facet_degree())),
                    std::next(std::begin(facet_points), 1),
                    [&vertex](const Polyhedron::Halfedge & h)
                    {
                        return Point_2(h.vertex()->point().x(), h.vertex()->point().y());
                    }
                );

                /**
                 * >> Plane equation
                 */
                Plane plane_equation(
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
                    return facet.is_perpendicular();
                }
            ),
            std::end(facets)
        );

        /** 
         * >> Heuristic sorting in order to have the minimum number of occlusions to deal with;
         */
        SimpleHeuristic heuristic;
        std::sort(std::begin(facets), std::end(facets), heuristic);

        std::copy(std::begin(facets), std::end(facets), std::ostream_iterator<projection::FacePrint>(std::cout, " \n"));

        return facets;
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
                        Polygon_with_holes first_parts_occluded, second_parts_occluded;
                        /**
                         * >> Assign Intersection to corresponding facet
                         */
                        std::for_each(
                            std::begin(superposition),
                            std::end(superposition),
                            [&lhs, &rhs, &first_parts_occluded, &second_parts_occluded](Polygon_with_holes intersection)
                            {
                                Point_2 intersection_point(
                                    CGAL::centroid(
                                        intersection.outer_boundary()[0],
                                        intersection.outer_boundary()[1],
                                        intersection.outer_boundary()[2]
                                        )
                                ); 
                                if(lhs.get_plane_height(intersection_point) > rhs.get_plane_height(intersection_point))
                                    CGAL::join(second_parts_occluded, intersection, second_parts_occluded);
                                else
                                    CGAL::join(first_parts_occluded, intersection, first_parts_occluded);
                            }
                        );
                        /**
                         * >> Compute oclusion for each facet
                         */
                        std::list<Polygon_with_holes> _firsts;
                        CGAL::difference(first, first_parts_occluded, std::back_inserter(_firsts));
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
                        CGAL::difference(second, second_parts_occluded, std::back_inserter(_seconds));
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
