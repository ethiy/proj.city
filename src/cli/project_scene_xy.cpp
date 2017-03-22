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
        std::cout << "Parsing arguments... ";
        std::map<std::string, docopt::value> arguments = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,
            "project_scene_xy 0.0.0-dev"
        );
        std::stringstream sconverter(arguments.at("--pixel_size").asString());
        double pixel_size(1);
        sconverter >> pixel_size;
        std::cout << "Done" << std::endl;

        boost::filesystem::path input_path(arguments.at("<filename>").asString());
        boost::filesystem::path root(input_path.parent_path());
        boost::filesystem::path scene_tree_file(root / (input_path.stem().string() + ".XML"));
                std::cout << scene_tree_file.string() << std::endl;

        tinyxml2::XMLDocument scene_tree;
        auto error = scene_tree.LoadFile(scene_tree_file.string().c_str());
        if(error != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("Could not read Pivot Point");

        double  x_offset(0),
                y_offset(0),
                z_offset(0);
        
        error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_x")->QueryDoubleText(&x_offset);
        if(error != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("Could not read Pivot Point");
        error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_y")->QueryDoubleText(&y_offset);
        if(error != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("Could not read Pivot Point");
        error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_z")->QueryDoubleText(&z_offset);
        if(error != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("Could not read Pivot Point");
        
        urban::shadow::Point pivot(x_offset, y_offset, z_offset);

        std::cout << "The shadow point : " << pivot << std::endl;

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
            std::begin(urban_objects),
            std::end(urban_objects),
            std::back_inserter(projections_xy),
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

        urban::io::FileHandler<GDALDriver> rasta("GTiff", boost::filesystem::path(root / (input_path.stem().string() + ".geotiff")), modes);
        urban::io::FileHandler<GDALDriver> victor("GML", boost::filesystem::path(root / (input_path.stem().string() + ".gml")), modes);
        std::cout << "rasterizing projections... ";
        urban::projection::RasterPrint rastafari = urban::rasterize(scene_projection, pixel_size, pivot);
        std::cout << "Done" << std::endl;

        std::cout << "Saving projections... ";
        victor.write(scene_projection);
        rasta.write(rastafari);
        std::cout << "Done" << std::endl;
    }
    catch(const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
