#include "config.h"

#include <urban.h>

#include <docopt.h>
#include <arguments.h>

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
      project_scene_xy <filename> [--pixel_size=<size> --rasterize --buildings --graphs --labels]
      project_scene_xy (-h | --help)
      project_scene_xy --version
    Options:
      -h --help             Show this screen.
      --version             Show version.
      --pixel_size=<size>   Pixel size [default: 1].
      --rasterize           Rasterize projections.
      --buildings           Save projections per building.
      --graphs              Save the Facets dual graph for buildings.
      --labels              Save vector projections with error fields.
)";


int main(int argc, const char** argv)
{
    try
    {
        std::cout << "Parsing arguments... " << std::flush;
        Arguments arguments(
            docopt::docopt(
                USAGE,
                { argv + 1, argv + argc },
                true,
                "project_scene_xy " + std::string(VERSION)
            )
        );
        std::cout << std::boolalpha << arguments << std::endl;
        std::cout << "Done." << std::flush << std::endl;

        boost::filesystem::path root(arguments.input_path.parent_path());

        std::cout << "Parsing scene tree... " << std::flush;
        urban::scene::Scene scene(
            urban::io::FileHandler<tinyxml2::XMLDocument>(
                boost::filesystem::path(root / (arguments.input_path.stem().string() + ".XML"))
            ).read()
        );
        auto building_ids = scene.identifiers();
        std::cout << "Done." << std::flush << std::endl;

        std::cout << "Reading Scene Meshes... " << std::flush;
        urban::io::FileHandler<Lib3dsFile> mesh_file(
            arguments.input_path,
            std::map<std::string,bool>{{"read", true}}
        );
        std::cout << " Done." << std::flush << std::endl;

        std::cout << "Loading Buildings... " << std::flush;
        std::vector<urban::scene::Building> buildings(building_ids.size());
        std::transform(
            std::begin(building_ids),
            std::end(building_ids),
            std::begin(buildings),
            [&mesh_file, scene](std::string const& id)
            {
                std::vector<urban::shadow::Mesh> meshes = mesh_file.read(id);

                meshes.erase(
                    std::remove_if(
                        std::begin(meshes),
                        std::end(meshes),
                        [](urban::shadow::Mesh const& mesh)
                        {
                            return mesh.get_name().at(0) == 'F';
                        }
                    ),
                    std::end(meshes)
                );

                urban::scene::Building building(
                    id,
                    urban::stitch(meshes),
                    scene.get_pivot(),
                    scene.get_epsg()
                );
                return urban::prune(building);
            }
        );
        std::cout << buildings.size() << " Done" << std::flush << std::endl;

        if(arguments.graphs)
        {
            std::cout << "Saving brick duals... " << std::flush;
            boost::filesystem::path dual_dir(root / "dual_graphs");
            boost::filesystem::create_directory(dual_dir);
            for(auto const& building : buildings)
            {
                std::fstream adjacency_file(
                    boost::filesystem::path(dual_dir / (arguments.input_path.stem().string() + "_" + building.get_name() + ".txt")).string(),
                    std::ios::out
                );
                urban::io::Adjacency_stream as(adjacency_file);
                as << building;
            }
            std::cout << " Done." << std::flush << std::endl;
        }


        std::cout << "Projecting... " << std::flush;
        std::vector<urban::projection::BrickPrint> projections_xy(buildings.size());
        std::transform(
            std::begin(buildings),
            std::end(buildings),
            std::begin(projections_xy),
            [](urban::scene::Building const& building)
            {
                return urban::project(building);
            }
        );
        std::cout << "Done." << std::flush << std::endl;

        std::cout << "Summing Projections... " << std::flush;
        urban::projection::BrickPrint scene_projection(scene.get_pivot());
        std::size_t i=0;
        for(auto projection : projections_xy)
        {
            scene_projection += projection;
            i++;
            std::cout << i << std::endl;
        }
        std::cout << "Done." << std::flush << std::endl;
        
        std::cout << "Saving vector projections... " << std::flush;
        urban::io::FileHandler<GDALDriver>(
            urban::io::GdalFormat::gml,
            boost::filesystem::path(root / (arguments.input_path.stem().string() + ".gml")),
            std::map<std::string,bool>{{"write", true}}
        ).write(scene_projection);
        if(arguments.buildings)
        {
            boost::filesystem::path vector_dir(root / "vectors");
            boost::filesystem::create_directory(vector_dir);
            for(auto const& projection : projections_xy)
            {
                urban::io::FileHandler<GDALDriver>(
                    urban::io::GdalFormat::gml,
                    boost::filesystem::path(vector_dir / (arguments.input_path.stem().string() + "_" + projection.get_name() + ".gml")),
                    std::map<std::string,bool>{{"write", true}}
                ).write(projection);
            }
            if(arguments.labels)
            {
                boost::filesystem::path label_dir(root / "labels");
                boost::filesystem::create_directory(label_dir);
                for(auto const& projection : projections_xy)
                {
                    urban::io::FileHandler<GDALDriver>(
                        urban::io::GdalFormat::shapefile,
                        boost::filesystem::path(label_dir / (arguments.input_path.stem().string() + "_" + projection.get_name() + ".shp")),
                        std::map<std::string,bool>{{"write", true}}
                    ).write(projection, true);
                }
            }
        }

        std::cout << "Done." << std::flush << std::endl;


        if(arguments.rasterize)
        {
            std::cout << "rasterizing projections... " << std::flush;
            urban::projection::RasterPrint global_rasta = urban::rasterize(scene_projection, arguments.pixel_size, scene.get_pivot());
            std::vector<urban::projection::RasterPrint> raster_projections(projections_xy.size());
            std::transform(
                std::begin(projections_xy),
                std::end(projections_xy),
                std::begin(raster_projections),
                [&scene, &arguments](urban::projection::BrickPrint const& projection)
                {
                    return urban::rasterize(projection, arguments.pixel_size, scene.get_pivot());
                }
            );
            std::cout << "Done." << std::flush << std::endl;

            if(arguments.buildings)
            {
                std::cout << "Saving raster projections... " << std::flush;
                urban::io::FileHandler<GDALDriver>(
                    urban::io::GdalFormat::geotiff,
                    boost::filesystem::path(root / (arguments.input_path.stem().string() + ".tiff")),
                    std::map<std::string,bool>{{"write", true}}
                ).write(global_rasta);
                boost::filesystem::path raster_dir(root / "rasters");
                boost::filesystem::create_directory(raster_dir);
                for(auto const& rasta : raster_projections)
                {
                    urban::io::FileHandler<GDALDriver>(
                        urban::io::GdalFormat::geotiff,
                        boost::filesystem::path(raster_dir / (arguments.input_path.stem().string() + "_" + rasta.get_name() + ".tiff")),
                        std::map<std::string,bool>{{"write", true}}
                    ).write(rasta);
                }
                std::cout << "Done." << std::flush << std::endl;
            }
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
