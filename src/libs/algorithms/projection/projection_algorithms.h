#pragma once

#include "../../geometry_definitions.h"
#include "../../scene/unode.h"
#include "../../scene/scene.h"
#include "../../projection/projection.h"

#include <vector>

namespace urban
{
    /** Projects on camera view*/
    projection::BrickPrint project(const scene::UNode & unode, const projection::Camera & camera);

    /** Projects on xy axis*/
    projection::BrickPrint project(const scene::UNode & unode);

    /** Projects on xy axis*/
    projection::BrickPrint project(const scene::Scene & urban_scene);

    projection::BrickPrint & project(projection::BrickPrint & projection, std::vector<scene::UNode> const& unodes);

    /** Project Faces to XY*/
    std::vector<projection::FacePrint> project_xy(const scene::UNode & unode);

    /** Computes FacePrint occlusions and gets rid of all perpendicular facets*/
    std::vector<projection::FacePrint> occlusion(const projection::FacePrint & lhs, std::vector<projection::FacePrint> & rhs);
}
