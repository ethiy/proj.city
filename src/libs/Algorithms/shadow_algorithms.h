#pragma once

#include "../Shadow/mesh.h"

namespace urban
{
    /** Stitch two meshes */
    bool stitch(const shadow::Mesh & lhs, const shadow::Mesh & rhs, shadow::Mesh & stitched);
}
