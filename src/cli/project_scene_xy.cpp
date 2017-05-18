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
        urban::scene::Scene scene = scene_tree_file.read();
        std::cout << "Done." << std::flush << std::endl;

        std::cout << "Reading Scene Meshes... " << std::flush;
        std::vector<urban::shadow::Mesh> meshes = urban::io::FileHandler<Lib3dsFile>(arguments.input_path, std::map<std::string,bool>{{"read", true}}).read();
        std::cout << meshes.size() << " meshes ; Done." << std::flush << std::endl;
        std::map<std::size_t, std::pair<std::vector<urban::shadow::Mesh>, std::vector<urban::shadow::Mesh> > > building_meshes = scene.cluster(meshes);
        for(auto & building_mesh : building_meshes)
        {
            building_mesh.second.erase(
                std::remove_if(
                    std::begin(building_mesh.second),
                    std::end(building_mesh.second),
                    [](urban::shadow::Mesh const& mesh)
                    {
                        return mesh.get_name().at(0) == 'F';
                    }
                ),
                std::end(building_mesh.second)
            );
            building_mesh.second = urban::stitch(building_mesh.second);
        }
        std::cout << "Done" << std::flush << std::endl;

        std::cout << "Loading Buildings... " << std::flush;
        auto pivot = scene.get_pivot();
        auto epsg_code = scene.get_epsg();
        std::vector<urban::scene::Building> buildings(building_meshes.size());
        std::transform(
            std::begin(building_meshes),
            std::end(building_meshes),
            std::begin(buildings),
            [&pivot, epsg_code](std::pair<std::size_t, std::vector<urban::shadow::Mesh> > const& building_mesh)
            {
                urban::scene::Building building(building_mesh.first, building_mesh.second, pivot, epsg_code);
                return urban::prune(building);
            }
        );
        std::cout << buildings.size() << " Done" << std::flush << std::endl;

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
        if(arguments.labels)
        {
            boost::filesystem::path label_dir(root / "labels");
            boost::filesystem::create_directory(label_dir);
            for(auto const& projection : projections_xy)
            {
                urban::io::FileHandler<GDALDriver> labelers(
                    "ESRI shapefile",
                    boost::filesystem::path(label_dir / (arguments.input_path.stem().string() + "_" + projection.get_name() + ".shp")),
                    modes
                );
                labelers.write(projection);
            }
        }
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
