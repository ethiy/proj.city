#pragma once

#include "../../scene/building.h"

namespace urban
{
    /** Prunes surfaces of buildings.
     * @param building building to prune
     * @return scene::Building with pruned surfaces
     */
    scene::Building & prune(scene::Building & building);
}
