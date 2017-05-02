#include "../libs/urban.h"

#include <docopt.h>

#include <boost/filesystem.hpp>

#include <tinyxml2.h>

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>

static const char USAGE[]=
R"(project_scene_xy.

    Usage:
      project_scene_xy <filename> [--pixel_size=<size>]
      project_scene_xy (-h | --help)
      project_scene_xy --version
    Options:
      -h --help             Show this screen.
      --version             Show version.
      --pixel_size=<size>   Pixel size [default: 1].
)";

int main(int argc, const char** argv)
{
    try
    {
        std::cout << "Parsing arguments... " << std::flush;
        std::map<std::string, docopt::value> arguments = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,
            "project_scene_xy 0.0.0-dev"
        );
        std::stringstream sconverter(arguments.at("--pixel_size").asString());
        double pixel_size(1);
        sconverter >> pixel_size;
        std::cout << "Done" << std::flush << std::endl;

        boost::filesystem::path input_path(arguments.at("<filename>").asString());
        boost::filesystem::path root(input_path.parent_path());
        boost::filesystem::path scene_tree_filepath(root / (input_path.stem().string() + ".XML"));

        urban::io::FileHandler<tinyxml2::XMLDocument> scene_tree_file(scene_tree_filepath);
        urban::scene::Scene scene = scene_tree_file.read();

        urban::io::FileHandler<Lib3dsFile> handler(input_path, std::map<std::string,bool>{{"read", true}});

        std::cout << "Reading and Stitching Scene Meshes... " << std::flush;
        std::vector<urban::shadow::Mesh> meshes = handler.read();
        std::map<std::size_t, std::vector<urban::shadow::Mesh> > building_meshes = scene.cluster(meshes);
        for(auto & building_mesh : building_meshes)
        {
            building_mesh.second = urban::stitch(building_mesh.second);
        }
        std::cout << "Done" << std::flush << std::endl;

        std::cout << "Loading Buildings... " << std::flush;
        auto pivot = scene.get_pivot();
        std::vector<urban::scene::Building> buildings(building_meshes.size());
        std::transform(
            std::begin(building_meshes),
            std::end(building_meshes),
            std::begin(buildings),
            [&pivot](std::pair<std::size_t, std::vector<urban::shadow::Mesh> > const& building_mesh)
            {
                urban::scene::Building building(building_mesh.first, building_mesh.second, pivot);
                return urban::prune(building);
            }
        );
        std::cout << buildings.size() << " Done" << std::flush << std::endl;

        // std::cout << "Saving brick duals... " << std::flush;
        // boost::filesystem::path dual_dir(root / "dual_graphs");
        // boost::filesystem::create_directory(dual_dir);
        // std::for_each(
        //     std::begin(urban_objects),
        //     std::end(urban_objects),
        //     [&dual_dir, &input_path](urban::scene::Brick const& brick)
        //     {
        //         std::fstream adjacency_file(boost::filesystem::path(dual_dir / (input_path.stem().string() + "_" + brick.get_name() + ".txt")).string(), std::ios::out);
        //         urban::io::Adjacency_stream as(adjacency_file);
        //         as << brick;
        //     }
        // );
        // std::cout << " Done" << std::flush << std::endl;


        // std::cout << "Projecting on XY... " << std::flush;
        // std::vector<urban::projection::BrickPrint> projections_xy(urban_objects.size());
        // std::transform(
        //     std::begin(urban_objects),
        //     std::end(urban_objects),
        //     std::begin(projections_xy),
        //     [](urban::scene::Brick const& brick)
        //     {
        //         return urban::project(brick);
        //     }
        // );
        // std::cout << "Done" << std::flush << std::endl;

        // std::cout << "Summing Projections... " << std::flush;
        // urban::projection::BrickPrint scene_projection(pivot);
        // for(auto projection : projections_xy)
        // {
        //     scene_projection += projection;
        // }
        // std::cout << "Done" << std::flush << std::endl;

        // std::map<std::string,bool>&& modes{{"write", true}};
        
        // std::cout << "Saving vector projections... " << std::flush;
        // urban::io::FileHandler<GDALDriver> victor("GML", boost::filesystem::path(root / (input_path.stem().string() + ".gml")), modes);
        // victor.write(scene_projection);
        // boost::filesystem::path vector_dir(root / "vectors");
        // boost::filesystem::create_directory(vector_dir);
        // std::for_each(
        //     std::begin(projections_xy),
        //     std::end(projections_xy),
        //     [&vector_dir, &input_path, &modes](urban::projection::BrickPrint const& projection)
        //     {
        //         urban::io::FileHandler<GDALDriver> victors(
        //             "GML",
        //             boost::filesystem::path(vector_dir / (input_path.stem().string() + "_" + projection.get_name() + ".gml")),
        //             modes
        //         );
        //         victors.write(projection);
        //     }
        // );

        // std::cout << "Done" << std::flush << std::endl;


        // std::cout << "rasterizing projections... " << std::flush;
        // urban::projection::RasterPrint global_rasta = urban::rasterize(scene_projection, pixel_size, pivot);
        // std::vector<urban::projection::RasterPrint> raster_projections(projections_xy.size());
        // std::transform(
        //     std::begin(projections_xy),
        //     std::end(projections_xy),
        //     std::begin(raster_projections),
        //     [&pivot, pixel_size](urban::projection::BrickPrint const& projection)
        //     {
        //         return urban::rasterize(projection, pixel_size, pivot);
        //     }
        // );
        // std::cout << "Done" << std::flush << std::endl;

        // std::cout << "Saving raster projections... " << std::flush;
        // urban::io::FileHandler<GDALDriver> rastaf("GTiff", boost::filesystem::path(root / (input_path.stem().string() + ".tiff")), modes);
        // rastaf.write(global_rasta);
        // boost::filesystem::path raster_dir(root / "rasters");
        // boost::filesystem::create_directory(raster_dir);
        // std::for_each(
        //     std::begin(raster_projections),
        //     std::end(raster_projections),
        //     [&raster_dir, &input_path, &modes](urban::projection::RasterPrint const& rasta)
        //     {
        //         urban::io::FileHandler<GDALDriver> rastafari(
        //             "GTiff",
        //             boost::filesystem::path(raster_dir / (input_path.stem().string() + "_" + rasta.get_name() + ".tiff")),
        //             modes
        //         );
        //         rastafari.write(rasta);
        //     }
        // );
        // std::cout << "Done" << std::flush << std::endl;
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
