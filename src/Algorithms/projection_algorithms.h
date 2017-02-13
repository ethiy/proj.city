#pragma once

#include "../geometry_definitions.h"
#include "../UrbanObject/brick.h"
#include "../Projection/face_projection.h"
#include "../Projection/brick_projection.h"
#include "../Projection/camera.h"

#include <vector>
#include <list>

namespace urban
{
    /*________________________ Utilities _________________________________________________*/
    /** Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation rotation_transform(const std::map<double, Vector> & _rotations);

    /** Check points colinearity*/
    bool check_colinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last);

    /** Get extrems from coliear points*/
    void extrem_points(std::vector<Point_2> & points);

    /** Compute FaceProjection area*/
    double area(const FaceProjection & facet);

    /*________________________ Projections _________________________________________________*/
    /** Projects on camera view*/
    BrickProjection project(Brick & brick, projection::Camera & camera);

    /* ! Projects on xy axis*/
    BrickProjection project(const Brick & brick);
    
    /** Project Faces to XY*/
    std::vector<FaceProjection> project_xy(const Brick & brick, bool keep_all);

    /** Computes FaceProjection occlutions and gets rid of all perpendicular facets*/
    std::list<FaceProjection> occlusion(FaceProjection & lhs, FaceProjection & rhs);
}
