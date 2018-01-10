#pragma once

#include <geometry_definitions.h>
#include <scene/unode.h>

#include <vector>


namespace urban
{
    /*! Computes border length*/
    double border_length(scene::UNode const& unode);

    /*! Applies affine transformations to unodes*/
    scene::UNode & affine_transform(scene::UNode &, const Affine_transformation_3 &);
    /*! Translate unodes*/
    scene::UNode & translate(scene::UNode &, const Vector_3 &);
    /*! Scale unodes*/
    scene::UNode & scale(scene::UNode &, double);
    /*! Rotate unodes*/
    scene::UNode & rotate(scene::UNode &, const Vector_3 &, double);
    /*! Rotate unodes*/
    scene::UNode & rotate(scene::UNode &, const std::map<double, Vector_3> &);

    /**
     * Prunes surfaces of unode.
     * @param unode the surface to prune
     * @return the surface with pruned facets
     */
    scene::UNode & prune(scene::UNode & unode);

    /**
     * Computes the unode surface area
     * @param unode surface of which the area is computed
     * @return the surface area
    */
    double area(scene::UNode const& unode);

    /**
     * Computes the unode surface total edge length
     * @param unode surface of which the total edge length is computed
     * @return the unode surface total edge length
    */
    double total_edge_length(scene::UNode const& unode);
}
