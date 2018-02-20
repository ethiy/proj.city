#include <config.h>

#include <urban.h>

#include <docopt.h>

#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <ostream>
#include <string>

static const char USAGE[]=
R"(orthoproject.

    Usage:
      orthoproject <scene> --input-format=<input_frmt> [--prune --cache --graphs --terrain --with-xml] [save --projection-format=<projection_frmt> --scene --labels] [rasterize --pixel-size=<size>]
      orthoproject (-h | --help)
      orthoproject --version
    Options:
      -h --help                             Show this screen.
      --version                             Show version.
      --prune                               Prune building faces.
      --cache                               Save buildings.
      --with-xml                            Read using the XML scene description.
      --input-format=<input_frmt>           Specify input format.
      --graphs                              Save the building facets dual graph.
      --projection-format=<proj_frmt>       Projection format [default: ESRI Shapefile].
      --scene                               Sum and save the scene projection.
      --labels                              Save vector projections with error fields.
      --terrain                             Taking care of terrain.
      --pixel-size=<size>                   Pixel size [default: 1].
)";

struct Arguments
{
    struct SceneArguments
    {
        boost::filesystem::path input_path;
        std::string input_format;
        bool prune = false;
        bool cache = false;
        bool graphs = false;
        bool terrain = false;
        bool with_xml = false;
    };
    struct SavingArguments
    {
        bool projections = false;
        std::string projection_format;
        bool scene = false;
        bool labels = false;

        bool saving(void)
        {
            return projections;
        }
    };
    struct RasterizingArguments
    {
        double pixel_size = 0;

        bool rasterizing(void)
        {
            return static_cast<bool>(pixel_size);
        }
    };

    Arguments(std::map<std::string, docopt::value> const& docopt_args)
    {
        std::cout << "Parsing arguments... " << std::flush;
        
        scene_args.input_path = docopt_args.at("<scene>").asString();
        scene_args.input_format = docopt_args.at("--input-format").asString();
        scene_args.prune = docopt_args.at("--prune").asBool();
        scene_args.cache = docopt_args.at("--cache").asBool();
        scene_args.graphs = docopt_args.at("--graphs").asBool();
        scene_args.terrain = docopt_args.at("--terrain").asBool();
        scene_args.with_xml = docopt_args.at("--with-xml").asBool();
        
        save_args.projections = docopt_args.at("save").asBool();
        if(save_args.projections)
        {
            save_args.projection_format = docopt_args.at("--projection-format").asString();
            save_args.scene = docopt_args.at("--scene").asBool();
            save_args.labels = docopt_args.at("--labels").asBool();
        }

        if(docopt_args.at("rasterize").asBool())
            raster_args.pixel_size = std::stod(docopt_args.at("--pixel-size").asString());
        
        std::cout << "Done." << std::flush << std::endl;
    }
    ~Arguments(void)
    {}

    SceneArguments scene_args;
    SavingArguments save_args;
    RasterizingArguments raster_args;
};

inline std::ostream & operator <<(std::ostream & os, Arguments & arguments)
{
    os << "Arguments:" << std::endl
       << "  Input path: " << arguments.scene_args.input_path << std::endl
       << "  Input format: " << arguments.scene_args.input_format << std::endl
       << "  Pruning faces: " << arguments.scene_args.prune << std::endl
       << "  Caching buildings: " << arguments.scene_args.cache << std::endl
       << "  Taking care of terrain: " << arguments.scene_args.terrain << std::endl
       << "  Reading scene from XML: " << arguments.scene_args.with_xml << std::endl
       << "  Saving dual graphs: " << arguments.scene_args.graphs << std::endl
       << "  Saving: " << arguments.save_args.saving() << std::endl
       << "     Saving projections: " << arguments.save_args.projections << std::endl
       << "     Projection saving format: " << arguments.save_args.projection_format << std::endl
       << "     Summing over whole scene: " << arguments.save_args.scene << std::endl
       << "     Saving projection error fields: " << arguments.save_args.labels << std::endl
       << "  Rasterizing: " << arguments.raster_args.rasterizing() << std::endl
       << "     Pixel size: " << arguments.raster_args.pixel_size << std::endl;
    return os;
}


urban::scene::Scene input(Arguments::SceneArguments const& scene_args)
{
    urban::scene::Scene scene;
    boost::filesystem::path data_directory(scene_args.input_path.parent_path());
    
    std::cout << "Parsing scene tree... " << std::flush;
    if(urban::io::format(scene_args.input_format) == "3DS")
        scene = urban::load_3ds_scene(scene_args.input_path, scene_args.with_xml);
    std::cout << "Done." << std::flush << std::endl;

    std::cout << "Processing scene... " << std::flush;
    if(scene_args.prune)
        urban::prune(scene, scene_args.terrain);
    
    if(scene_args.cache)
        urban::save_scene(data_directory, scene);

    if(scene_args.graphs)
        urban::save_building_duals(data_directory, scene);
    std::cout << "Done." << std::flush << std::endl;

    return scene;
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

        auto scene = input(arguments.scene_args);
        
        boost::filesystem::path data_directory(arguments.scene_args.input_path.parent_path());

        if(arguments.save_args.saving())
        {
            auto projections = urban::orthoproject(scene, arguments.scene_args.terrain);

            urban::save_building_prints(data_directory, projections, arguments.save_args.labels);

            if(arguments.save_args.scene)
                urban::save_scene_prints(
                    data_directory,
                    arguments.scene_args.input_path.stem().string(),
                    projections,
                    arguments.raster_args.rasterizing(),
                    arguments.raster_args.pixel_size
                );
                    
            if(arguments.raster_args.rasterizing())
            {
                auto raster_projections = urban::rasterize_scene(projections, arguments.raster_args.pixel_size);
                urban::save_building_rasters(data_directory, raster_projections);
            }
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
