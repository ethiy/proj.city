#pragma once

#include "../geometry_definitions.h"
#include "../Projection/face_projection.h"

namespace urban
{
    /*! Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation rotation_transform(const std::map<double, Vector> &);
}