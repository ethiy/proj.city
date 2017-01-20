#pragma once

#include "../UrbanObject/geometry_definitions.h"
#include "../UrbanObject/brick.h"

namespace urban
{
    /*! Computes brick barycenter*/
    Point barycenter(Brick &);

    /*! Returns Bounding Box*/
    Bbox bound(Brick &);

    /*! Projects on camera view*/
    // CGAL::Polygon_2<Kernel> project(Brick &, Camera &);

    /*! Computes border length*/
    double border_length(Brick &);

    /*! Applies affine transformations to bricks*/
    void affine_transform(Brick &, Affine_transformation &);
    /*! Translate bricks*/
    void translate(Brick &, Vector &);
    /*! Scale bricks*/
    void scale(Brick &, double);
    /*! Rotate bricks*/
    void rotate(Brick &, double);

    /*! Computes plane equations*/
    void plane_equations(Brick &);

    /*! Computes brick surface area*/
    double area(Brick &);
}