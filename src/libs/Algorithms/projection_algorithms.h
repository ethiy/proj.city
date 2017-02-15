#pragma once

#include "../geometry_definitions.h"
#include "../UrbanObject/brick.h"
#include "../Projection/projection.h"

#include <vector>
#include <list>

namespace urban
{
    /** Projects on camera view*/
    projection::BrickPrint project(const Brick & brick, const projection::Camera & camera);

    /* ! Projects on xy axis*/
    projection::BrickPrint project(const Brick & brick);
    
    /** Project Faces to XY*/
    std::vector<projection::FacePrint> project_xy(const Brick & brick);

    /** Computes FacePrint occlutions and gets rid of all perpendicular facets*/
    std::list<projection::FacePrint> occlusion(const projection::FacePrint & lhs, std::list<projection::FacePrint> & rhs);
}
