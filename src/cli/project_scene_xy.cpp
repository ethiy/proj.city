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
        std::cout << "Done" << std::flush << std::endl;

        boost::filesystem::path input_path(arguments.at("<filename>").asString());
        boost::filesystem::path root(input_path.parent_path());
        boost::filesystem::path scene_tree_file(root / (input_path.stem().string() + ".XML"));
                std::cout << scene_tree_file.string() << std::flush << std::endl;

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

        std::cout << "The shadow point : " << pivot << std::flush << std::endl;

        std::map<std::string,bool> modes{{"write", true}, {"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(input_path, modes);
        std::cout << "Reading Scene Meshes... ";
        std::vector<urban::shadow::Mesh> meshes = handler.read();
        std::cout << "Done" << std::flush << std::endl;

        std::cout << "Filtering Meshes... ";
        std::vector<urban::shadow::Mesh> roofs;
        meshes.erase(
            std::remove_if(
                std::begin(meshes),
                std::end(meshes),
                [](const urban::shadow::Mesh & mesh)
                {
                    return mesh.get_name().at(0) == 'F' || mesh.get_name().at(0) == 'M';
                }
            ),
            std::end(meshes)
        );
        std::cout << "Done" << std::flush << std::endl;

        std::cout << "Stitching roofs... ";
        std::vector<urban::shadow::Mesh> stitched_roofs = urban::stitch(meshes);
        std::cout << stitched_roofs.size() << " Done" << std::flush << std::endl;

        std::cout << "Loading to Scene Bricks... ";
        std::vector<urban::Brick> urban_objects(stitched_roofs.size());
        std::transform(
            std::begin(stitched_roofs),
            std::end(stitched_roofs),
            std::begin(urban_objects),
            [](const urban::shadow::Mesh & mesh)
            {
                return urban::Brick(mesh);
            }
        );
        std::cout << urban_objects.size() << " Done" << std::flush << std::endl;

        std::cout << "Projecting on XY... ";
        std::vector<urban::projection::BrickPrint> projections_xy(urban_objects.size());
        std::transform(
            std::begin(urban_objects),
            std::end(urban_objects),
            std::begin(projections_xy),
            [](const urban::Brick & brick)
            {
                return urban::project(brick);
            }
        );
        std::cout << "Done" << std::flush << std::endl;

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
        std::cout << "Done" << std::flush << std::endl;
        
        std::cout << "Saving vector projections... ";
        urban::io::FileHandler<GDALDriver> victor("GML", boost::filesystem::path(root / (input_path.stem().string() + ".gml")), modes);
        victor.write(scene_projection);
        std::for_each(
            std::begin(projections_xy),
            std::end(projections_xy),
            [&root, &input_path, &modes](const urban::projection::BrickPrint & projection)
            {
                urban::io::FileHandler<GDALDriver> victors(
                    "GML",
                    boost::filesystem::path(root / (input_path.stem().string() + "_" + projection.get_name() + ".gml")),
                    modes
                );
                victors.write(projection);
            }
        );

        std::cout << "Done" << std::flush << std::endl;


        std::cout << "rasterizing projections... ";
        urban::projection::RasterPrint global_rasta = urban::rasterize(scene_projection, pixel_size, pivot);
        std::vector<urban::projection::RasterPrint> raster_projections(projections_xy.size());
        std::transform(
            std::begin(projections_xy),
            std::end(projections_xy),
            std::begin(raster_projections),
            [&pivot, pixel_size](const urban::projection::BrickPrint & projection)
            {
                return urban::rasterize(projection, pixel_size, pivot);
            }
        );
        std::cout << "Done" << std::flush << std::endl;

        std::cout << "Saving raster projections... ";
        urban::io::FileHandler<GDALDriver> rastaf("GTiff", boost::filesystem::path(root / (input_path.stem().string() + ".tiff")), modes);
        rastaf.write(global_rasta);
        std::for_each(
            std::begin(raster_projections),
            std::end(raster_projections),
            [&root, &input_path, &modes](const urban::projection::RasterPrint & rasta)
            {
                urban::io::FileHandler<GDALDriver> rastafari(
                    "GTiff",
                    boost::filesystem::path(root / (input_path.stem().string() + "_" + rasta.get_name() + ".tiff")),
                    modes
                );
                rastafari.write(rasta);
            }
        );
        std::cout << "Done" << std::flush << std::endl;
    }
    catch(const std::exception& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
