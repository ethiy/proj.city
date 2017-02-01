#include "projection_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <vector>
#include <iterator>
#include <algorithm>

#include <cmath>

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

    std::map<size_t, FaceProjection> project_xy(Brick & brick)
    {
        std::map<size_t, FaceProjection> facets;
        std::vector<Point_2> facet_points(brick.vertices_number());
        size_t index(0);
        std::for_each(
            brick.facets_begin(),
            brick.facets_end(),
            [&facets, index, facet_points](Facet & facet) mutable
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
                Facet::Halfedge_handle halfedge = facet.halfedge();
                facets[index++] = FaceProjection(   Polygon_with_holes( Polygon(    std::begin(facet_points), // if all colinear just extreems!!
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

    void occlusion(FaceProjection & lhs, FaceProjection & rhs)
    {
        Polygon_with_holes first(lhs.get_polygon()), second(rhs.get_polygon());
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
        }
    }
}
