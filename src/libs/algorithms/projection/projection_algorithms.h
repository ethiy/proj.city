#pragma once

#include "../../geometry_definitions.h"
#include "../../scene/building.h"
#include "../../scene/brick/brick.h"
#include "../../projection/projection.h"

#include <vector>
#include <list>

namespace urban
{
    /** Projects on camera view*/
    projection::BrickPrint project(const scene::Brick & brick, const projection::Camera & camera);

    /** Projects on xy axis*/
    projection::BrickPrint project(const scene::Brick & brick);

    /** Projects on xy axis*/
    projection::BrickPrint project(const scene::Building & building);

    projection::BrickPrint & project(projection::BrickPrint & projection, std::vector<scene::Brick> const& bricks);

    /** Project Faces to XY*/
    std::vector<projection::FacePrint> project_xy(const scene::Brick & brick);

    /** Computes FacePrint occlusions and gets rid of all perpendicular facets*/
    std::list<projection::FacePrint> occlusion(const projection::FacePrint & lhs, std::list<projection::FacePrint> & rhs);
}
