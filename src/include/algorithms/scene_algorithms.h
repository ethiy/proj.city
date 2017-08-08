#pragma once

#include <scene/scene.h>

#include <io/Adjacency_stream/adjacency_stream.h>

#include <projection/scene_projection.h>

#include <boost/filesystem/path.hpp>

namespace urban
{
    void save_building_duals(boost::filesystem::path const& root_path, urban::scene::Scene const& scene);

    std::vector<urban::projection::FootPrint> orthoproject(urban::scene::Scene const& scene);
}
