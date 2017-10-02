#pragma once

#include <scene/scene.h>

#include <projection/scene_projection.h>

#include <boost/filesystem/path.hpp>

namespace urban
{
    void save_scene(boost::filesystem::path const& root_path, scene::Scene const& scene);
    void save_building_duals(boost::filesystem::path const& root_path, scene::Scene const& scene);
    void save_building_prints(boost::filesystem::path const& root_path, std::vector<projection::FootPrint> const& projections, bool const labels);
    void save_building_rasters(boost::filesystem::path const& root_path, std::vector<projection::RasterPrint> const& raster_projections);
    void save_scene_prints(boost::filesystem::path const& root_path, std::string const& filename, std::vector<projection::FootPrint> const& projections, bool const rasterize, double const pixel_size);

    std::vector<projection::FootPrint> orthoproject(scene::Scene const& scene);
    std::vector<projection::RasterPrint> rasterize_scene(std::vector<projection::FootPrint> const& projections, double const  pixel_size);
    scene::Scene & prune(scene::Scene & scene);
}
