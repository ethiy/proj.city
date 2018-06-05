#include <algorithms/scene_algorithms.h>

#include <io/Adjacency_stream/adjacency_stream.h>

#include <io/io_raster.h>
#include <io/io_vector.h>

#include <io/io_scene.h>

namespace city
{
    void save_building_duals(boost::filesystem::path const& root_path, scene::Scene const& scene)
    {
        std::cout << "Saving brick duals... " << std::flush;
        boost::filesystem::path dual_dir(root_path / "dual_graphs");
        boost::filesystem::create_directory(dual_dir);
        for(auto const& building : scene)
        {
            try
            {
                std::fstream adjacency_file(
                    boost::filesystem::path(dual_dir / (building.get_name() + ".txt")).string(),
                    std::ios::out
                );
                io::Adjacency_stream as(adjacency_file);
                as << building;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        std::cout << " Done." << std::flush << std::endl;
    }
    void save_building_rasters(boost::filesystem::path const& root_path, std::vector<projection::RasterPrint> const& raster_projections)
    {
        std::cout << "Saving raster projections... " << std::flush;
        boost::filesystem::path raster_dir(root_path / "rasters");
        boost::filesystem::create_directory(raster_dir);
        for(auto const& rasta : raster_projections)
        {
            city::io::RasterHandler(
                boost::filesystem::path(raster_dir / (rasta.get_name() + ".tiff")),
                std::map<std::string,bool>{{"write", true}}
            ).write(rasta);
        }
        std::cout << "Done." << std::flush << std::endl;
    }
}
