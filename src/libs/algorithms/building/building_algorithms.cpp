#include "building_algorithms.h"

#include "../brick/brick_algorithms.h"

namespace urban
{
    scene::Building & prune(scene::Building & building)
    {
        for(auto & brick : building)
        {
            brick = prune(brick);
        }
        return building;
    }
}
