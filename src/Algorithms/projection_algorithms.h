#pragma once

#include "../geometry_definitions.h"
#include "../Projection/face_projection.h"
#include "../UrbanObject/brick.h"

#include <vector>

namespace urban
{
    /*! Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation rotation_transform(const std::map<double, Vector> &);

    /*! Check points colinearity*/
    bool check_colinearity(std::vector<Point_2>::iterator, std::vector<Point_2>::iterator);

    /*! Get extrems from coliear points*/
    void extrem_points(std::vector<Point_2> &);

    /*! Project Faces to XY*/
    std::vector<FaceProjection> project_xy(Brick &);

    /*! Compute FaceProjection area*/
    double area(FaceProjection &);

    /*! Computes FaceProjection occlutions*/
    std::vector<FaceProjection> occlusion(FaceProjection &, FaceProjection &);
}
