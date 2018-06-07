#include <config.h>

#include <city.h>

#include <docopt.h>

#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

#include <numeric>
#include <functional>

#include <ostream>
#include <string>

static const char USAGE[]=
R"(orthoproject.

    Usage:
      orthoproject <scene>... --input-format=<input_frmt> [--prune --read-xml --terrain] --save=<output_path> [--graphs --scene --labels] [--rasterize --pixel-size=<size>]
      orthoproject (-h | --help)
      orthoproject --version
    Options:
      -h --help                             Show this screen.
      --version                             Show version.
      --input-format=<input_frmt>           Specify input format.
      --prune                               Prune building faces.
      --read-xml                            Read using XML scene tree file.
      --graphs                              Save the building facets dual graph.
      --terrain                             Taking care of terrain.
      --save=<output_path>                  Specify output path.
      --scene                               Sum and save the scene projection.
      --labels                              Save vector projections with error fields.
      --rasterize                           Rasterize projections.
      --pixel-size=<size>                   Pixel size [default: 1].
)";

struct Arguments
{
    struct SceneArguments
    {
        std::vector<boost::filesystem::path> input_paths;
        std::string input_format;
        bool prune = false;
        bool xml = false;
        bool terrain = false;
    };
    struct SavingArguments
    {
        boost::filesystem::path output_path;
        bool graphs = false;
        bool scene = false;
        bool labels = false;
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
        
        auto path_strings = docopt_args.at("<scene>").asStringList();
        scene_args.input_paths = std::vector<boost::filesystem::path>(path_strings.size());
        std::transform(
            std::begin(path_strings),
            std::end(path_strings),
            std::begin(scene_args.input_paths),
            [](std::string const& string_path)
            {
                return boost::filesystem::path(string_path);
            }
        );
        scene_args.input_format = docopt_args.at("--input-format").asString();
        scene_args.prune = docopt_args.at("--prune").asBool();
        scene_args.xml = docopt_args.at("--read-xml").asBool();
        scene_args.terrain = docopt_args.at("--terrain").asBool();
        
        save_args.output_path = docopt_args.at("--save").asString();
        save_args.graphs = docopt_args.at("--graphs").asBool();
        save_args.scene = docopt_args.at("--scene").asBool();
        save_args.labels = docopt_args.at("--labels").asBool();

        if(docopt_args.at("--rasterize").asBool())
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
       << "  Input paths: " << std::endl;
    for(auto const& path : arguments.scene_args.input_paths)
        os << "    " << path.string() << std::endl;
    os << "  Input format: " << arguments.scene_args.input_format << std::endl
       << "  Pruning faces: " << arguments.scene_args.prune << std::endl
       << "  Read Scene tree: " << arguments.scene_args.xml << std::endl
       << "  Taking care of terrain: " << arguments.scene_args.terrain << std::endl
       << "  Saving: " << std::endl
       << "     Output path: " << arguments.save_args.output_path << std::endl
       << "     Dual graphs: " << arguments.save_args.graphs << std::endl
       << "     Summing over whole scene: " << arguments.save_args.scene << std::endl
       << "     Projection error fields: " << arguments.save_args.labels << std::endl
       << "  Rasterizing: " << arguments.raster_args.rasterizing() << std::endl
       << "     Pixel size: " << arguments.raster_args.pixel_size << std::endl;
    return os;
}


city::scene::Scene input_scene(Arguments::SceneArguments const& scene_args, Arguments::SavingArguments const& save_args);

city::scene::Scene input_scene(Arguments::SceneArguments const& scene_args, Arguments::SavingArguments const& save_args)
{
    auto scene = tbb::parallel_reduce(
        tbb::blocked_range<std::vector<boost::filesystem::path>::const_iterator>(
            std::begin(scene_args.input_paths),
            std::end(scene_args.input_paths)
        ),
        city::scene::Scene(),
        [&scene_args](tbb::blocked_range<std::vector<boost::filesystem::path>::const_iterator> const& b_range, city::scene::Scene const& init)
        {
            return std::accumulate(
                std::begin(b_range),
                std::end(b_range),
                init,
                [&scene_args](city::scene::Scene const& whole_scene, boost::filesystem::path const& scene_path)
                {
                    return  whole_scene
                            +
                            city::io::SceneHandler(
                                scene_path,
                                std::map<std::string, bool>{{"read", true}},
                                scene_args.input_format,
                                scene_args.xml
                            ).read();
                }
            );
        },
        std::plus<city::scene::Scene>()
    );    

    if(scene_args.prune)
        scene = city::scene::prune(scene);
    
    if(save_args.graphs)
        city::save_building_duals(
            save_args.output_path,
            scene
        );
    
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

        if(!boost::filesystem::is_directory(arguments.save_args.output_path))
            boost::filesystem::create_directory(arguments.save_args.output_path);

        auto scene = input_scene(arguments.scene_args, arguments.save_args);

        city::projection::ScenePrint scene_projection(scene);

        city::io::SceneVectorHandler(arguments.save_args.output_path / "vectors", scene_projection).write(arguments.save_args.labels);
                
        if(arguments.raster_args.rasterizing())
        {
            auto raster_projections = city::projection::rasterize(scene_projection, arguments.raster_args.pixel_size);
            city::save_building_rasters(arguments.save_args.output_path, raster_projections);
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
