#include "brick_algorithms.h"

#include <CGAL/aff_transformation_tags.h>

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <cmath>

namespace urban
{
    double border_length(Brick & brick)
    {
        return std::accumulate(
            brick.halfedges_begin(),
            brick.halfedges_end(),
            .0,
            [](double & length, const Polyhedron::Halfedge & halfedge)
            {
                return length + halfedge.is_border() * std::sqrt(to_double(Vector(halfedge.next()->vertex()->point(), halfedge.vertex()->point()) * Vector(halfedge.next()->vertex()->point(), halfedge.vertex()->point())));
            }
        );
    }

    void affine_transform(Brick & brick, Affine_transformation & affine_transformation)
    {
        std::transform(
            brick.points_begin(),
            brick.points_end(),
            brick.points_begin(),
            [& affine_transformation](Point & point)
            {
                return affine_transformation.transform(point);
            }
        );
    }

    void translate(Brick & brick, const Vector & offset)
    {
        Affine_transformation translation(CGAL::TRANSLATION, offset);
        affine_transform(brick, translation);
    }

    void scale(Brick & brick, double scale)
    {
        Affine_transformation scaling(CGAL::SCALING, scale);
        affine_transform(brick, scaling);
    }

    void rotate(Brick & brick, Vector & axis, double angle)
    {
        double norm(to_double(axis*axis));
        norm = std::sqrt(norm);
        Vector u(axis/norm);
        double m00(std::cos(angle) + (1 - std::cos(angle)) * std::pow(to_double(u.x()), 2.));
        double m01((1 - std::cos(angle)) * to_double(u.x()) * to_double(u.y()) - std::sin(angle) * to_double(u.z()));
        double m02((1 - std::cos(angle)) * to_double(u.x()) * to_double(u.z()) + std::sin(angle) * to_double(u.y()));
        double m10((1 - std::cos(angle)) * to_double(u.x()) * to_double(u.y()) + std::sin(angle) * to_double(u.z()));
        double m11(std::cos(angle) + (1 - std::cos(angle)) * std::pow(to_double(u.y()), 2.));
        double m12((1 - std::cos(angle)) * to_double(u.y()) * to_double(u.z()) - std::sin(angle) * to_double(u.x()));
        double m20((1 - std::cos(angle)) * to_double(u.x()) * to_double(u.z()) - std::sin(angle) * to_double(u.y()));
        double m21((1 - std::cos(angle)) * to_double(u.y()) * to_double(u.z()) + std::sin(angle) * to_double(u.x()));
        double m22(std::cos(angle) + (1 - std::cos(angle)) * std::pow(to_double(u.z()), 2.));
        Affine_transformation rotation(m00, m01, m02, m10, m11, m12, m20, m21, m22);
        affine_transform(brick, rotation);
    }

    void plane_equations(Brick& brick)
    {
        std::transform(
            brick.facets_begin(),
            brick.facets_end(),
            brick.planes_begin(),
            [](Facet & facet)
            {
                Facet::Halfedge_handle halfedge = facet.halfedge();
                return Facet::Plane_3(
                    halfedge->vertex()->point(),
                    halfedge->next()->vertex()->point(),
                    halfedge->next()->next()->vertex()->point()
                );
            }
        );
    }

    double area(Brick& brick)
    {
        return std::accumulate(
            brick.facets_begin(),
            brick.facets_end(),
            .0,
            [](double & area, Facet & facet)
            {
                Polyhedron::Halfedge_around_facet_circulator h = facet.facet_begin();
                Vector normal = CGAL::normal(h->vertex()->point(), h->next()->vertex()->point(), h->next()->next()->vertex()->point());
                return area + std::accumulate(
                    facet.facet_begin(),
                    std::next(facet.facet_begin(), facet.facet_degree()),
                    .0,
                    [normal](double & surface_area, const Polyhedron::Halfedge & halfedge)
                    {
                        return surface_area + to_double(CGAL::cross_product(halfedge.vertex()->point() - CGAL::ORIGIN, halfedge.next()->vertex()->point() - CGAL::ORIGIN) * normal/2.);;
                    }
                );
            }
        );
    }
}