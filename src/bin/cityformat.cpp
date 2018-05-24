#include <config.h>

#include <urban.h>

#include <docopt.h>

#include <boost/filesystem.hpp>

static const char USAGE[]=
R"(cityformat.

    Usage:
      cityformat output --output-format=<output_format> <output_path> [--graphs --write-xml] <scene>... --input-format=<input_frmt> [--read-xml --prune --terrain]
      cityformat --formats
      cityformat (-h | --help)
      cityformat --version
    Options:
      -h --help                             Show this screen.
      --version                             Show version.
      --read-xml                            Read using XML scene tree file.
      --prune                               Prune building faces.
      --input-format=<input_frmt>           Specify input format.
      --graphs                              Save the building facets dual graph.
      --terrain                             Taking care of terrain.
      --write-xml                           Write XML scene tree file.
      --output-format=<output_frmt>         Specify output format.
      --formats                             Give all possible formats.
)";

struct Arguments
{
    struct SceneArguments
    {
        std::vector<boost::filesystem::path> input_paths;
        std::string input_format;
        bool xml = false;
        bool prune = false;
        bool terrain = false;
    };
    struct SaveArguments
    {
        boost::filesystem::path output_path;
        bool xml = true;
        bool graphs = false;
        std::string output_format;
    };

    Arguments(std::map<std::string, docopt::value> const& docopt_args)
        : formats(docopt_args.at("--formats").asBool())
    {
        std::cout << "Parsing arguments... " << std::flush;
        
        if(!formats)
        {
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
            scene_args.xml = docopt_args.at("--read-xml").asBool();
            scene_args.prune = docopt_args.at("--prune").asBool();
            scene_args.terrain = docopt_args.at("--terrain").asBool();
            
            save_args.output_path = docopt_args.at("<output_path>").asString();
            save_args.xml = docopt_args.at("--write-xml").asBool();
            save_args.graphs = docopt_args.at("--graphs").asBool();
            save_args.output_format = docopt_args.at("--output-format").asString();
        }

        std::cout << "Done." << std::flush << std::endl;
    }
    ~Arguments(void)
    {}

    bool formats;
    SceneArguments scene_args;
    SaveArguments save_args;
};

inline std::ostream & operator <<(std::ostream & os, Arguments & arguments)
{
    if(arguments.formats)
        os << "Arguments:" << std::endl
           << "  Possible formats: " << arguments.formats << std::endl;
    else
        os << "Arguments:" << std::endl
           << "  Input paths: " << std::endl;
        for(auto const& path : arguments.scene_args.input_paths)
            os << "  " << path.string() << std::endl;
        os << "  Input format: " << arguments.scene_args.input_format << std::endl
           << "  Read Scene tree: " << arguments.scene_args.xml << std::endl
           << "  Pruning faces: " << arguments.scene_args.prune << std::endl
           << "  Taking care of terrain: " << arguments.scene_args.terrain << std::endl
           << "  Output path: " << arguments.save_args.output_path << std::endl
           << "  Saving dual graphs: " << arguments.save_args.graphs << std::endl
           << "  Write Scene tree: " << arguments.save_args.xml << std::endl
           << "  Output format: " << arguments.save_args.output_format << std::endl;

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

        if(arguments.formats)
        {
            std::cout << "Possible formats are: ";
            std::copy(
                std::begin(city::io::SceneHandler::supported_formats),
                std::prev(std::end(city::io::SceneHandler::supported_formats)),
                std::ostream_iterator<std::string>(std::cout, ", ")
            );
            std::cout << city::io::SceneHandler::supported_formats.back() << std::endl;
        }
        else
        {
            auto scene = std::accumulate(
                std::begin(arguments.scene_args.input_paths),
                std::end(arguments.scene_args.input_paths),
                city::scene::Scene(),
                [&arguments](city::scene::Scene const& whole_scene, boost::filesystem::path const& scene_path)
                {
                    return  whole_scene
                            +
                            city::io::SceneHandler(
                                scene_path,
                                std::map<std::string, bool>{{"read", true}},
                                arguments.scene_args.input_format,
                                arguments.scene_args.xml
                            ).read();
                }
            );

            if(arguments.scene_args.prune)
                scene = scene.prune(arguments.scene_args.terrain);
            
            if(arguments.save_args.graphs)
                city::save_building_duals(
                    arguments.save_args.output_path.parent_path(),
                    scene
                );

            city::io::SceneHandler scene_writer(
                arguments.save_args.output_path,
                std::map<std::string, bool>{{"write", true}},
                arguments.save_args.output_format,
                arguments.save_args.xml

            );
            scene_writer.write(scene);
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
