#include "brick_algorithms.h"

#include <CGAL/aff_transformation_tags.h>

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

namespace urban
{
    Point barycenter(Brick & brick)
    {
        Vector barycenter(0.0, 0.0, 0.0);
        std::cerr << "Nor yet implemented!" << std::endl;
        return CGAL::ORIGIN + barycenter;
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

    void plane_equations(Brick& brick)
    {
        std::transform(
            brick.facets_begin(),
            brick.facets_end(),
            brick.planes_begin(),
            [](Facet & facet)
            {
                Facet::Halfedge_handle h = facet.halfedge();
                return Facet::Plane_3(
                    h->vertex()->point(),
                    h->next()->vertex()->point(),
                    h->next()->next()->vertex()->point()
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
                double surface_area(0);
                size_t order(0);
                Polyhedron::Halfedge_around_facet_circulator h = facet.facet_begin();
                Vector normal = CGAL::normal(h->vertex()->point(), h->next()->vertex()->point(), h->next()->next()->vertex()->point());
                do
                {
                    surface_area += to_double(CGAL::cross_product(h->vertex()->point() - CGAL::ORIGIN, h->next()->vertex()->point() - CGAL::ORIGIN) * normal/2.);
                    ++ order;
                } while ( ++h != facet.facet_begin());
                if(order < 3)
                    throw new std::out_of_range("A Facet by construction must have at least 3 vertices.");
                return area + surface_area;
            }
        );
    }
}