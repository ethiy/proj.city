#include <config.h>

#include <urban.h>

#include <docopt.h>
#include <arguments.h>

#include <CGAL/IO/Geomview_stream.h>

#include <boost/filesystem.hpp>

#include <tinyxml2.h>

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>

static const char USAGE[]=
R"(orthoproject.

    Usage:
      orthoproject <scene> [--pixel_size=<size> --rasterize --buildings --graphs --labels --no_sum --prune]
      orthoproject (-h | --help)
      orthoproject --version
    Options:
      -h --help             Show this screen.
      --version             Show version.
      --pixel_size=<size>   Pixel size [default: 1].
      --rasterize           Rasterize projections.
      --buildings           Save projections per building.
      --graphs              Save the Facets dual graph for buildings.
      --labels              Save vector projections with error fields.
      --no_sum              Do not save the scene projection.
      --prune               Prune building faces.
)";

int main(int argc, const char** argv)
{
    try
    {
        Arguments arguments(
            docopt::docopt(
                USAGE,
                { argv + 1, argv + argc },
                true,
                "orthoproject " + std::string(VERSION)
            )
        );
        std::cout << std::boolalpha << arguments << std::endl;

        boost::filesystem::path root(arguments.input_path.parent_path());

        std::cout << "Parsing scene tree... " << std::flush;
        urban::scene::Scene scene(
            urban::io::FileHandler<tinyxml2::XMLDocument>(
                boost::filesystem::path(root / (arguments.input_path.stem().string() + ".XML"))
            ).read(
                urban::io::FileHandler<Lib3dsFile>(
                    arguments.input_path,
                    std::map<std::string,bool>{{"read", true}}
                )
            )
        );
        if(arguments.prune)
            urban::prune(scene);
        std::cout << "Done." << std::flush << std::endl;

        if(arguments.graphs)
            urban::save_building_duals(root, scene);
        
        auto projections = urban::orthoproject(scene);

        if(!arguments.no_sum)
            urban::save_scene_prints(root, arguments.input_path.stem().string(), projections, arguments.rasterize, arguments.pixel_size);
        
        if(arguments.buildings)
        {
            urban::save_building_prints(root, projections, arguments.labels);
            if(arguments.rasterize)
            {
                auto raster_projections = urban::rasterize_scene(projections, arguments.pixel_size);
                urban::save_building_rasters(root, raster_projections);
            }
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
