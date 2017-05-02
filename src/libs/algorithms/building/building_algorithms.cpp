#include "building_algorithms.h"

#include "../brick/brick_algorithms.h"

namespace urban
{
    scene::Building & prune(scene::Building & building)
    {
        for(auto const& brick : building)
        {
            brick = prune(brick);
        }
        return building;
    }
}
