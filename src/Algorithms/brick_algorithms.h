#pragma once

#include "../geometry_definitions.h"
#include "../UrbanObject/brick.h"
#include "../Projection/brick_projection.h"

namespace urban
{
    /*! Computes brick barycenter*/
    Point barycenter(Brick &);

    /*! Projects on camera view*/
    BrickProjection project(Brick &, Camera &);

    /* ! Projects on xy axis*/
    BrickProjection project(Brick &);

    /*! Computes border length*/
    double border_length(Brick &);

    /*! Applies affine transformations to bricks*/
    void affine_transform(Brick &, Affine_transformation &);
    /*! Translate bricks*/
    void translate(Brick &, Vector &);
    /*! Scale bricks*/
    void scale(Brick &, double);
    /*! Rotate bricks*/
    void rotate(Brick &, Vector &, double);

    /*! Computes plane equations*/
    void plane_equations(Brick &);

    /*! Computes brick surface area*/
    double area(Brick &);
}