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
    void save_building_prints(boost::filesystem::path const& root_path, std::vector<projection::FootPrint> const& projections, bool const labels)
    {
        std::cout << "Saving vector projections... " << std::flush;
        boost::filesystem::path vector_dir(root_path / "vectors");
        boost::filesystem::create_directory(vector_dir);
        for(auto const& projection : projections)
        {
            io::VectorHandler(
                boost::filesystem::path(vector_dir / (projection.get_name() + ".shp")),
                std::map<std::string,bool>{{"write", true}}
            ).write(projection, labels);

            std::fstream attributes_file(
                boost::filesystem::path(vector_dir / (projection.get_name() + ".txt")).string(),
                std::ios::out
            );

            auto areas = city::areas(projection);
            auto edges = city::edge_lengths(projection);

            std::copy(std::begin(areas), std::end(areas), std::ostream_iterator<double>(attributes_file, " "));
            attributes_file << std::endl;
            std::copy(std::begin(edges), std::end(edges), std::ostream_iterator<double>(attributes_file, " "));
            attributes_file << std::endl;
            attributes_file.close();
        }
        std::cout << "Done." << std::flush << std::endl;
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
    void save_scene_prints(boost::filesystem::path const& root_path, std::string const& filename, std::vector<projection::FootPrint> const& projections, bool const rasterize, double const pixel_size)
    {
        std::cout << "Summing , rasterizing and saving scene projections... " << std::flush;

        auto scene_projection = std::accumulate(
            std::begin(projections),
            std::end(projections),
            city::projection::FootPrint()
        );

        city::io::VectorHandler(
            boost::filesystem::path(root_path / (filename + ".shp")),
            std::map<std::string,bool>{{"write", true}}
        ).write(scene_projection);

        if(rasterize)
        {
            city::projection::RasterPrint global_rasta(scene_projection, pixel_size);

            city::io::RasterHandler(
                boost::filesystem::path(root_path / (filename + ".tiff")),
                std::map<std::string,bool>{{"write", true}}
            ).write(global_rasta);
        }

        std::cout << "Done." << std::flush << std::endl;        
    }

    std::vector<projection::RasterPrint> rasterize_scene(std::vector<projection::FootPrint> const& projections, double const  pixel_size)
    {
        std::cout << "rasterizing projections... " << std::flush;
        std::vector<projection::RasterPrint> raster_projections(projections.size());
        std::transform(
            std::begin(projections),
            std::end(projections),
            std::begin(raster_projections),
            [pixel_size](projection::FootPrint const& projection)
            {
                return projection::RasterPrint(projection, pixel_size);
            }
        );
        std::cout << "Done." << std::flush << std::endl;

        return raster_projections;
    }
}
