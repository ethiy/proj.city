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

    bool check_colinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last)
    {
        if(std::distance(first, last) < 3)
            throw new std::out_of_range("There are less than 3 points the two iterators!");
        
        /* It suffices to check the first three points*/
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

    std::vector<FaceProjection> project_xy(Brick & brick)
    {
        std::vector<FaceProjection> facets;
        std::vector<Point_2> facet_points(brick.vertices_number());
        std::transform(
            brick.facets_begin(),
            brick.facets_end(),
            std::back_inserter(facets),
            [facet_points](Facet & facet) mutable
            {
                facet_points.clear();
                std::transform(
                    facet.facet_begin(),
                    std::next(facet.facet_begin(), static_cast<long>(facet.facet_degree())),
                    std::back_inserter(facet_points),
                    [](const Polyhedron::Halfedge & h)
                    {
                        Point vertex(h.vertex()->point());
                        return Point_2(to_double(vertex.x()), to_double(vertex.y()));
                    }
                );

                /*! If projected points are colinear then we store only the extremal points*/
                if(check_colinearity(std::begin(facet_points), std::end(facet_points)))
                    extrem_points(facet_points);

                Facet::Halfedge_handle halfedge = facet.halfedge();
                return FaceProjection(   Polygon_with_holes( Polygon(    std::begin(facet_points),
                                                                         std::end(facet_points)
                                                                    )
                                                           ),
                                         Facet::Plane(   halfedge->vertex()->point(),
                                                         halfedge->next()->vertex()->point(),
                                                         halfedge->next()->next()->vertex()->point()
                                                     )
                                     );
            }
        );
        return facets;
    }

    double area(FaceProjection & facet)
    {
        return std::accumulate(
                    facet.holes_begin(),
                    facet.holes_end(),
                    to_double(facet.outer_boundary().area()),
                    [](double & holes_area, Polygon hole)
                    {
                        return holes_area - to_double(hole.area());
                    }
                );
    }

    std::vector<FaceProjection> occlusion(FaceProjection & lhs, FaceProjection & rhs)
    {
        Polygon_with_holes first(lhs.get_polygon()), second(rhs.get_polygon());
        std::vector<FaceProjection> result;

        if(CGAL::do_intersect(first, second))
        {
            std::vector<Polygon_with_holes> superposition;
            CGAL::intersection(first, second, std::back_inserter(superposition));

            Polygon_with_holes lhs_occlusion, rhs_occlusion;

            std::for_each(
                std::begin(superposition),
                std::end(superposition),
                [&](Polygon_with_holes intersection)
                {
                    Point_2 intersection_point; // to set 
                    if(lhs.get_height(intersection_point) > rhs.get_height(intersection_point))
                        CGAL::join(rhs_occlusion, intersection, rhs_occlusion);
                    else
                        CGAL::join(lhs_occlusion, intersection, lhs_occlusion);
                }
            );
            
            std::vector<Polygon_with_holes> _firsts;
            CGAL::difference(first, lhs_occlusion, std::back_inserter(_firsts));
            std::transform(
                std::begin(_firsts),
                std::end(_firsts),
                std::back_inserter(result),
                [&lhs](Polygon_with_holes part)
                {
                    return FaceProjection(part, lhs.get_plane());
                }
            );

            std::vector<Polygon_with_holes> _seconds;
            CGAL::difference(second, rhs_occlusion, std::back_inserter(_seconds));
            std::transform(
                std::begin(_seconds),
                std::end(_seconds),
                std::back_inserter(result),
                [&rhs](Polygon_with_holes part)
                {
                    return FaceProjection(part, rhs.get_plane());
                }
            );
        }
        else
        {
            result.push_back(lhs);
            result.push_back(rhs);
        }
        return result;
    }
}
