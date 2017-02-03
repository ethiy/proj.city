#include "brick_algorithms.h"
#include "projection_algorithms.h"

#include <CGAL/aff_transformation_tags.h>

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <cmath>

namespace urban
{
    BrickProjection project(const Brick & brick)
    {
        BrickProjection projection(brick.get_name(), brick.bbox());
        std::vector<FaceProjection> projected_facets = project_xy(brick);
        std::for_each(
            std::begin(projected_facets),
            std::end(projected_facets),
            [&projection](FaceProjection facet)
            {
                projection.push_facet(facet);
            }
        );
        return projection;
    }

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

    void affine_transform(Brick & brick, const Affine_transformation & affine_transformation)
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

    void rotate(Brick & brick, const Vector & axis, double angle)
    {
        std::map<double, Vector> _rotation{{angle, axis}};
        Affine_transformation rotation(rotation_transform(_rotation));
        affine_transform(brick, rotation);
    }

    void rotate(Brick & brick, const std::map<double, Vector> & _rotations)
    {
        Affine_transformation rotation(rotation_transform(_rotations));
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
                return area 
                + to_double(CGAL::cross_product(h->vertex()->point() - CGAL::ORIGIN, h->next()->vertex()->point() - CGAL::ORIGIN) * normal/2.)
                + std::accumulate(
                    std::next(facet.facet_begin(), 1),
                    std::next(facet.facet_begin(), static_cast<long>(facet.facet_degree())),
                    .0,
                    [normal](double & surface_area, const Polyhedron::Halfedge & halfedge)
                    {
                        return surface_area + to_double(CGAL::cross_product(halfedge.vertex()->point() - CGAL::ORIGIN, halfedge.next()->vertex()->point() - CGAL::ORIGIN) * normal/2.);
                    }
                );
            }
        );
    }
}
