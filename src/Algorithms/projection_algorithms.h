#pragma once

#include "../geometry_definitions.h"
#include "../Projection/face_projection.h"
#include "../UrbanObject/brick.h"

#include <map>

namespace urban
{
    /*! Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation rotation_transform(const std::map<double, Vector> &);

    /*! Project Face to XY*/
    std::map<size_t, FaceProjection> project_xy(const Brick &);
}