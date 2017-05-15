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

struct Arguments
{
public:
    Arguments(std::map<std::string, docopt::value> const& docopt_args)
        : input_path(docopt_args.at("<filename>").asString()),
          rasterize(docopt_args.at("--rasterize").asBool()),
          buildings(docopt_args.at("--buildings").asBool()),
          graphs(docopt_args.at("--graphs").asBool()),
          labels(docopt_args.at("--labels").asBool())
    {
        std::stringstream sconverter(docopt_args.at("--pixel_size").asString());
        sconverter >> pixel_size;
    }
    ~Arguments(void)
    {}

    boost::filesystem::path input_path;
    double pixel_size = 1;
    bool rasterize = false;
    bool buildings = false;
    bool graphs = false;
    bool labels = false;
};

std::ostream & operator <<(std::ostream & os, Arguments & arguments)
{
    os << "Arguments:" << std::endl
       << "  Input path: " << arguments.input_path << std::endl
       << "  Pixel size: " << arguments.pixel_size << std::endl
       << "  Rasterize: " << arguments.rasterize << std::endl
       << "  Buildings: " << arguments.buildings << std::endl
       << "  Graphs: " << arguments.graphs << std::endl
       << "  Labels: " << arguments.labels << std::endl;
    return os;
}


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
        std::cout << "Done." << std::flush << std::endl;
        std::cout << std::boolalpha << arguments << std::endl;

        boost::filesystem::path root(arguments.input_path.parent_path());

        std::cout << "Parsing scene tree... " << std::flush;
        

        urban::io::FileHandler<tinyxml2::XMLDocument> scene_tree_file(
            boost::filesystem::path(root / (arguments.input_path.stem().string() + ".XML"))
        );
        urban::shadow::Point pivot = scene_tree_file.pivot();
        std::map<std::size_t, std::set<std::string> > building_clusters = scene_tree_file.buildings();
        std::cout << "Done." << std::flush << std::endl;
        std::cout << "The shadow point : " << pivot << std::flush << std::endl;

        std::cout << "Reading Scene Meshes... " << std::flush;
        std::vector<urban::shadow::Mesh> meshes = urban::io::FileHandler<Lib3dsFile>(arguments.input_path, std::map<std::string,bool>{{"read", true}}).read();
        std::cout << meshes.size() << " meshes ; Done." << std::flush << std::endl;

        std::map<std::size_t, std::vector<urban::shadow::Mesh> > buildings;

        std::vector<urban::shadow::Mesh> buffer_meshes;
        for(auto const& building_pair : building_clusters)
        {
            buffer_meshes.reserve(building_pair.second.size());
            std::transform(
                std::begin(building_pair.second),
                std::end(building_pair.second),
                std::back_inserter(buffer_meshes),
                [&meshes](std::string const& mesh_name)
                {
                    auto placeholder = std::find_if(
                        std::begin(meshes),
                        std::end(meshes),
                        [&mesh_name](urban::shadow::Mesh const& mesh)
                        {
                            return mesh.get_name() == mesh_name;
                        }
                    );
                    return *placeholder;
                }
            );
            buildings.emplace(building_pair.first, buffer_meshes);
            buffer_meshes.clear();
        }

        std::cout << "Filtering Meshes... " << std::flush;
        meshes.erase(
            std::remove_if(
                std::begin(meshes),
                std::end(meshes),
                [](urban::shadow::Mesh const& mesh)
                {
                    return mesh.get_name().at(0) == 'F' || mesh.get_name().at(0) == 'M';
                }
            ),
            std::end(meshes)
        );
        std::cout << "Done." << std::flush << std::endl;

        std::cout << "Stitching roofs... " << std::flush;
        std::vector<urban::shadow::Mesh> stitched_roofs = urban::stitch(meshes);
        std::cout << stitched_roofs.size() << " Done." << std::flush << std::endl;

        std::cout << "Loading to Scene Bricks... " << std::flush;
        std::vector<urban::Brick> urban_objects(stitched_roofs.size());
        std::transform(
            std::begin(stitched_roofs),
            std::end(stitched_roofs),
            std::begin(urban_objects),
            [&pivot](urban::shadow::Mesh const& mesh)
            {
                urban::Brick brick(mesh, pivot);
                return urban::prune(brick);
            }
        );
        std::cout << urban_objects.size() << " Done." << std::flush << std::endl;

        if(arguments.graphs)
        {
            std::cout << "Saving brick duals... " << std::flush;
            boost::filesystem::path dual_dir(root / "dual_graphs");
            boost::filesystem::create_directory(dual_dir);
            std::for_each(
                std::begin(urban_objects),
                std::end(urban_objects),
                [&dual_dir, &arguments](urban::Brick const& brick)
                {
                    std::fstream adjacency_file(boost::filesystem::path(dual_dir / (arguments.input_path.stem().string() + "_" + brick.get_name() + ".txt")).string(), std::ios::out);
                    urban::io::Adjacency_stream as(adjacency_file);
                    as << brick;
                }
            );
            std::cout << " Done." << std::flush << std::endl;
        }


        std::cout << "Projecting on XY... " << std::flush;
        std::vector<urban::projection::BrickPrint> projections_xy(urban_objects.size());
        std::transform(
            std::begin(urban_objects),
            std::end(urban_objects),
            std::begin(projections_xy),
            [](urban::Brick const& brick)
            {
                return urban::project(brick);
            }
        );
        std::cout << "Done." << std::flush << std::endl;

        std::cout << "Summing Projections... " << std::flush;
        urban::projection::BrickPrint scene_projection(pivot);
        for(auto projection : projections_xy)
        {
            scene_projection += projection;
        }
        std::cout << "Done." << std::flush << std::endl;

        std::map<std::string,bool>&& modes{{"write", true}};
        
        std::cout << "Saving vector projections... " << std::flush;
        urban::io::FileHandler<GDALDriver> victor("GML", boost::filesystem::path(root / (arguments.input_path.stem().string() + ".gml")), modes);
        victor.write(scene_projection);
        if(arguments.buildings)
        {
            boost::filesystem::path vector_dir(root / "vectors");
            boost::filesystem::create_directory(vector_dir);
            std::for_each(
                std::begin(projections_xy),
                std::end(projections_xy),
                [&vector_dir, &arguments, &modes](urban::projection::BrickPrint const& projection)
                {
                    urban::io::FileHandler<GDALDriver> victors(
                        "GML",
                        boost::filesystem::path(vector_dir / (arguments.input_path.stem().string() + "_" + projection.get_name() + ".gml")),
                        modes
                    );
                    victors.write(projection);
                }
            );
        }

        std::cout << "Done." << std::flush << std::endl;


        if(arguments.rasterize)
        {
            std::cout << "rasterizing projections... " << std::flush;
            urban::projection::RasterPrint global_rasta = urban::rasterize(scene_projection, arguments.pixel_size, pivot);
            std::vector<urban::projection::RasterPrint> raster_projections(projections_xy.size());
            std::transform(
                std::begin(projections_xy),
                std::end(projections_xy),
                std::begin(raster_projections),
                [&pivot, &arguments](urban::projection::BrickPrint const& projection)
                {
                    return urban::rasterize(projection, arguments.pixel_size, pivot);
                }
            );
            std::cout << "Done." << std::flush << std::endl;

            if(arguments.buildings)
            {
                std::cout << "Saving raster projections... " << std::flush;
                urban::io::FileHandler<GDALDriver> rastaf("GTiff", boost::filesystem::path(root / (arguments.input_path.stem().string() + ".tiff")), modes);
                rastaf.write(global_rasta);
                boost::filesystem::path raster_dir(root / "rasters");
                boost::filesystem::create_directory(raster_dir);
                std::for_each(
                    std::begin(raster_projections),
                    std::end(raster_projections),
                    [&raster_dir, &arguments, &modes](urban::projection::RasterPrint const& rasta)
                    {
                        urban::io::FileHandler<GDALDriver> rastafari(
                            "GTiff",
                            boost::filesystem::path(raster_dir / (arguments.input_path.stem().string() + "_" + rasta.get_name() + ".tiff")),
                            modes
                        );
                        rastafari.write(rasta);
                    }
                );
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
