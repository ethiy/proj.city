#pragma once

#include "../../geometry_definitions.h"
#include "../../brick/brick.h"


namespace urban
{
    /*! Computes border length*/
    double border_length(Brick &);

    /*! Applies affine transformations to bricks*/
    Brick & affine_transform(Brick &, const Affine_transformation_3 &);
    /*! Translate bricks*/
    Brick & translate(Brick &, const Vector_3 &);
    /*! Scale bricks*/
    Brick & scale(Brick &, double);
    /*! Rotate bricks*/
    Brick & rotate(Brick &, const Vector_3 &, double);
    /*! Rotate bricks*/
    Brick & rotate(Brick &, const std::map<double, Vector_3> &);

    /*! Computes plane equations*/
    void plane_equations(Brick &);

    /*! Computes brick surface area*/
    double area(Brick &);
}
