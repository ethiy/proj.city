#pragma once

#include <scene/scene.h>

#include <projection/scene_projection.h>

#include <boost/filesystem/path.hpp>

namespace city
{
    void save_building_duals(boost::filesystem::path const& root_path, scene::Scene const& scene);
    void save_building_rasters(boost::filesystem::path const& root_path, std::vector<projection::RasterPrint> const& raster_projections);
}
