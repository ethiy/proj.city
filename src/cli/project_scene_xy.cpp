#include "../libs/urban.h"

#include <docopt.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>

static const char USAGE[]=
R"(project_scene_xy.

    Usage:
      project_scene_xy <filename>
      project_scene_xy (-h | --help)
      project_scene_xy --version
    Options:
      -h --help     Show this screen.
      --version     Show version.
)";

int main(int argc, const char** argv)
{
    try
    {
        std::cout << "Parsing arguments... ";
        std::map<std::string, docopt::value> arguments = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,
            "project_scene_xy 0.0.0-dev"
        );

        std::cout << "Done" << std::endl;

        boost::filesystem::path input_path(arguments.at("<filename>").asString());

        std::map<std::string,bool> modes{{"write", true}, {"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(input_path, modes);
        std::cout << "Reading Scene Meshes... ";
        std::vector<urban::shadow::Mesh> meshes = handler.read();
        std::cout << "Done" << std::endl;

        std::cout << "Loading to Scene Bricks... ";
        std::vector<urban::Brick> urban_objects(meshes.size());
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::begin(urban_objects),
            [](const urban::shadow::Mesh & mesh)
            {
                return urban::Brick(mesh);
            }
        );
        std::cout << "Done" << std::endl;

        std::cout << "Projecting on XY... ";
        std::vector<urban::projection::BrickPrint> projections_xy(urban_objects.size());
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::begin(projections_xy),
            [](const urban::Brick & brick)
            {
                return urban::project(brick);
            }
        );
        std::cout << "Done" << std::endl;

        std::cout << "Summing Projections... ";
        urban::projection::BrickPrint scene_projection = std::accumulate(
            std::begin(projections_xy),
            std::end(projections_xy),
            urban::projection::BrickPrint(),
            [](urban::projection::BrickPrint & rhs, const urban::projection::BrickPrint & lhs)
            {
                return rhs + lhs;
            }
        );
        std::cout << "Done" << std::endl;

        boost::filesystem::path root(input_path.remove_leaf());
        urban::io::FileHandler<GDALDriver> rasta("GTiff", boost::filesystem::path(root / (input_path.stem().string() + ".geotiff")), modes);
        urban::io::FileHandler<GDALDriver> victor("GML", boost::filesystem::path(root / (input_path.stem().string() + ".gml")), modes);
        std::cout << "Saving projections... ";
        victor.write(scene_projection);
        rasta.write(urban::rasterize(scene_projection, .1, 8));
        std::cout << "Done" << std::endl;
    }
    catch(const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
