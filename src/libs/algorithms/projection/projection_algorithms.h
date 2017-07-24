#pragma once

#include "../../geometry_definitions.h"
#include "../../scene/unode.h"
#include "../../scene/scene.h"
#include "../../projection/projection.h"

#include <vector>

namespace urban
{
    /** Projects on camera view*/
    projection::BrickPrint project(scene::UNode const& unode, projection::Camera const& camera);

    /** Projects on xy axis*/
    projection::BrickPrint project(scene::UNode const& unode);

    /** Projects on xy axis*/
    projection::BrickPrint project(const scene::Scene & urban_scene);

    projection::BrickPrint & project(projection::BrickPrint & projection, std::vector<scene::UNode> const& unodes);

    /** Project Faces to XY*/
    std::vector<projection::FacePrint> project_xy(scene::UNode const& unode);
}
