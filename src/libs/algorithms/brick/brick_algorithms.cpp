#include "brick_algorithms.h"
#include "../utils/util_algorithms.h"

#include <CGAL/aff_transformation_tags.h>

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <cmath>

namespace urban
{
    double border_length(scene::Brick & brick)
    {
        return std::accumulate(
            brick.border_halfedges_begin(),
            brick.halfedges_end(),
            .0,
            [](double & length, const Polyhedron::Halfedge & halfedge)
            {
                return length + std::sqrt(to_double(Vector_3(halfedge.next()->vertex()->point(), halfedge.vertex()->point()) * Vector_3(halfedge.next()->vertex()->point(), halfedge.vertex()->point())));
            }
        );
    }

    scene::Brick & affine_transform(scene::Brick & brick, const Affine_transformation_3 & affine_transformation)
    {
        std::transform(
            brick.points_begin(),
            brick.points_end(),
            brick.points_begin(),
            [& affine_transformation](Point_3 & point)
            {
                return affine_transformation.transform(point);
            }
        );
        return brick;
    }

    scene::Brick & translate(scene::Brick & brick, const Vector_3 & offset)
    {
        Affine_transformation_3 translation(CGAL::TRANSLATION, offset);
        return affine_transform(brick, translation);
    }

    scene::Brick & scale(scene::Brick & brick, double scale)
    {
        Affine_transformation_3 scaling(CGAL::SCALING, scale);
        return affine_transform(brick, scaling);
    }

    scene::Brick & rotate(scene::Brick & brick, const Vector_3 & axis, double angle)
    {
        std::map<double, Vector_3> _rotation{{angle, axis}};
        Affine_transformation_3 rotation(rotation_transform(_rotation));
        return affine_transform(brick, rotation);
    }

    scene::Brick & rotate(scene::Brick & brick, const std::map<double, Vector_3> & _rotations)
    {
        Affine_transformation_3 rotation(rotation_transform(_rotations));
        return affine_transform(brick, rotation);
    }


    void plane_equations(scene::Brick& brick)
    {
        std::transform(
            brick.facets_begin(),
            brick.facets_end(),
            brick.planes_begin(),
            [](scene::Brick::Facet & facet)
            {
                scene::Brick::Halfedge_handle halfedge = facet.halfedge();
                return scene::Brick::Facet::Plane_3(
                    halfedge->vertex()->point(),
                    halfedge->next()->vertex()->point(),
                    halfedge->next()->next()->vertex()->point()
                );
            }
        );
    }

    scene::Brick & prune(scene::Brick & brick)
    {
        std::vector<scene::Brick::Halfedge_handle> prunables = brick.pruning_halfedges();
        for(auto prunable : prunables)
        {
            brick = brick.join_facet(prunable);
        }
        return brick;
    }

    std::vector<scene::Brick> & prune(std::vector<scene::Brick> & bricks)
    {
        std::transform(
            std::begin(bricks),
            std::end(bricks),
            std::begin(bricks),
            [](scene::Brick & brick)
            {
                return prune(brick);
            }
        );
        return bricks;
    }

    double area(scene::Brick& brick)
    {
        return std::accumulate(
            brick.facets_begin(),
            brick.facets_end(),
            .0,
            [&brick](double & area, scene::Brick::Facet & facet)
            {
                return area + brick.area(facet);
            }
        );
    }
}
