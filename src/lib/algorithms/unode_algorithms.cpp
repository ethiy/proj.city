#include <algorithms/unode_algorithms.h>
#include <algorithms/util_algorithms.h>

#include <CGAL/aff_transformation_tags.h>
#include <CGAL/squared_distance_3.h>

#include <boost/range/iterator_range.hpp>

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <cmath>

namespace city
{
    double border_length(scene::UNode const& unode)
    {
        double length(0);
        for(auto const& halfedge : unode.halfedges())
            if(unode.is_border(halfedge))
                length += std::sqrt(
                    to_double(
                        CGAL::squared_distance(
                            unode.point(unode.target(halfedge)),
                            unode.point(unode.source(halfedge))
                        )
                    )
                );
        return length;
    }

    scene::UNode& affine_transform(scene::UNode & unode, const Affine_transformation_3 & affine_transformation)
    {
        std::transform(
            std::begin(unode.points()),
            std::end(unode.points()),
            std::begin(unode.points()),
            [& affine_transformation](Point_3 & point)
            {
                return affine_transformation.transform(point);
            }
        );
        return unode;
    }

    scene::UNode & translate(scene::UNode & unode, const Vector_3 & offset)
    {
        Affine_transformation_3 translation(CGAL::TRANSLATION, offset);
        return affine_transform(unode, translation);
    }

    scene::UNode & scale(scene::UNode & unode, double scale)
    {
        Affine_transformation_3 scaling(CGAL::SCALING, scale);
        return affine_transform(unode, scaling);
    }

    scene::UNode & rotate(scene::UNode & unode, const Vector_3 & axis, double angle)
    {
        std::map<double, Vector_3> _rotation{{angle, axis}};
        Affine_transformation_3 rotation(rotation_transform(_rotation));
        return affine_transform(unode, rotation);
    }

    scene::UNode & rotate(scene::UNode & unode, const std::map<double, Vector_3> & _rotations)
    {
        Affine_transformation_3 rotation(rotation_transform(_rotations));
        return affine_transform(unode, rotation);
    }

    scene::UNode& prune(scene::UNode & unode)
    {
        for(auto const& halfedge : boost::make_iterator_range(UNode::generator_t(&unode.prunable_halfedges_generator()), UNode::generator_t()))
            unode = unode.join_facet(halfedge);
        return unode;
    }

    double area(scene::UNode const& unode)
    {
        return std::accumulate(
            std::begin(unode.faces()),
            std::end(unode.faces()),
            .0,
            [&unode](double area, scene::UNode::Face_index const& facet)
            {
                return area + unode.area(facet);
            }
        );
    }

    double total_edge_length(scene::UNode const& unode)
    {
        return std::accumulate(
            std::begin(unode.halfedges()),
            std::end(unode.halfedges()),
            .0,
            [&unode](double const total_length, Mesh::Halfedge_index const& halfedge)
            {
                return  total_length
                        +
                        std::sqrt(
                            to_double(
                                CGAL::squared_distance(
                                    unode.point(unode.target(halfedge)),
                                    unode.point(unode.source(halfedge))
                                )
                            )
                        );
            }
        );
    }
}
