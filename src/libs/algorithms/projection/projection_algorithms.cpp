#include "projection_algorithms.h"
#include "../utils/util_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    projection::BrickPrint project(scene::Scene const& scene)
    {
        // projection::BrickPrint projection(scene.get_name(), scene.bbox(), scene.pivot(), scene.get_epsg());
        // projection = scene.project_roofs(projection);
        return projection::BrickPrint();
    }

    projection::BrickPrint & project(projection::BrickPrint & projection, std::vector<scene::UNode> const& unodes)
    {
        projection = std::accumulate(
            std::begin(unodes),
            std::end(unodes),
            projection,
            [](projection::BrickPrint & sum, scene::UNode const& unode)
            {
                return sum + project(unode);
            }
        );
        return projection;
    }
}
