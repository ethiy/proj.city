#pragma once

#include "../../geometry_definitions.h"
#include "../../scene/brick/brick.h"

#include <vector>


namespace urban
{
    /*! Computes border length*/
    double border_length(scene::Brick &);

    /*! Applies affine transformations to bricks*/
    scene::Brick & affine_transform(scene::Brick &, const Affine_transformation_3 &);
    /*! Translate bricks*/
    scene::Brick & translate(scene::Brick &, const Vector_3 &);
    /*! Scale bricks*/
    scene::Brick & scale(scene::Brick &, double);
    /*! Rotate bricks*/
    scene::Brick & rotate(scene::Brick &, const Vector_3 &, double);
    /*! Rotate bricks*/
    scene::Brick & rotate(scene::Brick &, const std::map<double, Vector_3> &);

    /*! Computes plane equations*/
    void plane_equations(scene::Brick &);

    /** Prunes surfaces of brick.
     * @param brick brick to prune
     * @return scene::Brick with pruned surfaces
     */
    scene::Brick & prune(scene::Brick & brick);
    /** Prunes surfaces of bricks.
     * @param brick a vector of bricks to prune
     * @return a vector of bricks with pruned surfaces
     */
    std::vector<scene::Brick> & prune(std::vector<scene::Brick> & brick);

    /** Computes brick surface area*/
    double area(scene::Brick & brick);
}
