#include <config.h>

#include <urban.h>

#include <docopt.h>

#include <boost/filesystem.hpp>

#include <ostream>

static const char USAGE[]=
R"(orthoproject.

    Usage:
      orthoproject <scene> [--prune --buildings --graphs --save_projections --sum_projections --labels --rasterize --terrain --pixel_size=<size>]
      orthoproject (-h | --help)
      orthoproject --version
    Options:
      -h --help             Show this screen.
      --version             Show version.
      --prune               Prune building faces.
      --buildings           Save buildings.
      --graphs              Save the Facets dual graph per building.
      --save_projections    Save projections per building.
      --sum_projections     Sum and save the scene projection.
      --labels              Save vector projections with error fields.
      --rasterize           Rasterize and save projections.
      --terrain             Rasterize the terrain also.
      --pixel_size=<size>   Pixel size [default: 1].
)";

struct Arguments
{
public:
    Arguments(std::map<std::string, docopt::value> const& docopt_args)
        : input_path(docopt_args.at("<scene>").asString()),
          prune(docopt_args.at("--prune").asBool()),
          buildings(docopt_args.at("--buildings").asBool()),
          graphs(docopt_args.at("--graphs").asBool()),
          save_projections(docopt_args.at("--save_projections").asBool()),
          sum_projections(docopt_args.at("--sum_projections").asBool()),
          labels(docopt_args.at("--labels").asBool()),
          rasterize(docopt_args.at("--rasterize").asBool()),
          terrain(docopt_args.at("--terrain").asBool())
    {
        std::cout << "Parsing arguments... " << std::flush;
        std::stringstream sconverter(docopt_args.at("--pixel_size").asString());
        sconverter >> pixel_size;
        std::cout << "Done." << std::flush << std::endl;
    }
    ~Arguments(void)
    {}

    boost::filesystem::path input_path;
    bool prune = false;
    bool buildings = false;
    bool graphs = false;
    bool save_projections = false;
    bool sum_projections = false;
    bool labels = false;
    bool rasterize = false;
    bool terrain = false;
    double pixel_size = 1;
};

inline std::ostream & operator <<(std::ostream & os, Arguments & arguments)
{
    os << "Arguments:" << std::endl
       << "  Input path: " << arguments.input_path << std::endl
       << "  Prune faces: " << arguments.prune << std::endl
       << "  Buildings: " << arguments.buildings << std::endl
       << "  Graphs: " << arguments.graphs << std::endl
       << "  Save projections: " << arguments.save_projections << std::endl
       << "  Sum projections: " << arguments.sum_projections << std::endl
       << "  Save Labels: " << arguments.labels << std::endl
       << "  Rasterize: " << arguments.rasterize << std::endl
       << "  Terrain: " << arguments.terrain << std::endl
       << "  Pixel size: " << arguments.pixel_size << std::endl;
    return os;
}


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
        
        if(arguments.buildings)
            urban::save_scene(root, scene);
        std::cout << "Done." << std::flush << std::endl;

        if(arguments.graphs)
            urban::save_building_duals(root, scene);
        
        if(arguments.sum_projections || arguments.save_projections)
        {
            auto projections = urban::orthoproject(scene, arguments.terrain);
            
            if(arguments.sum_projections)
                urban::save_scene_prints(root, arguments.input_path.stem().string(), projections, arguments.rasterize, arguments.pixel_size);
                    
            if(arguments.save_projections)
            {
                urban::save_building_prints(root, projections, arguments.labels);
                if(arguments.rasterize)
                {
                    auto raster_projections = urban::rasterize_scene(projections, arguments.pixel_size);
                    urban::save_building_rasters(root, raster_projections);
                }
            }            
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
