#include "building_algorithms.h"

#include "../brick/brick_algorithms.h"

namespace urban
{
    scene::Building & prune(scene::Building & building)
    {
        building.prune_roofs();
        building.prune_walls();
        return building;
    }
}
