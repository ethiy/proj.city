#pragma once

#include "../geometry_definitions.h"
#include "../UrbanObject/brick.h"


namespace urban
{
    /*! Computes border length*/
    double border_length(Brick &);

    /*! Applies affine transformations to bricks*/
    void affine_transform(Brick &, const Affine_transformation &);
    /*! Translate bricks*/
    void translate(Brick &, const Vector &);
    /*! Scale bricks*/
    void scale(Brick &, double);
    /*! Rotate bricks*/
    void rotate(Brick &, const Vector &, double);
    /*! Rotate bricks*/
    void rotate(Brick &, const std::map<double, Vector> &);

    /*! Computes plane equations*/
    void plane_equations(Brick &);

    /*! Computes brick surface area*/
    double area(Brick &);
}
