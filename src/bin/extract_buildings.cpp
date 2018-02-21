#include <config.h>

#include <urban.h>

#include <docopt.h>

#include <boost/filesystem.hpp>

static const char USAGE[]=
R"(extract_buildings.

    Usage:
      extract_buildings <scene> [--prune --with-xml --terrain]
      extract_buildings (-h | --help)
      extract_buildings --version
    Options:
      -h --help             Show this screen.
      --version             Show version.
      --prune               Prune building faces.
      --terrain             Taking care of terrain.
      --with-xml            Read using the XML scene description
)";

struct Arguments
{
public:
    Arguments(std::map<std::string, docopt::value> const& docopt_args)
        : input_path(docopt_args.at("<scene>").asString()),
          prune(docopt_args.at("--prune").asBool()),
          terrain(docopt_args.at("--terrain").asBool()),
          with_xml(docopt_args.at("--with-xml").asBool())
    {}
    ~Arguments(void)
    {}

    boost::filesystem::path input_path;
    bool prune = false;
    bool terrain = false;
    bool with_xml = false;
};

inline std::ostream & operator <<(std::ostream & os, Arguments & arguments)
{
    os << "Arguments:" << std::endl
       << "  Input path: " << arguments.input_path << std::endl
       << "  Prune faces: " << arguments.prune << std::endl
       << "  Taking care of terrain: " << arguments.terrain << std::endl
       << "  With Xml scene description: " << arguments.with_xml << std::endl;
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

        boost::filesystem::path data_directory(arguments.input_path.parent_path());

        std::cout << "Parsing scene tree... " << std::flush;

        urban::scene::Scene scene;
        
        if(arguments.with_xml)
            scene = urban::io::FileHandler<tinyxml2::XMLDocument>(
                    boost::filesystem::path(data_directory / (arguments.input_path.stem().string() + ".XML"))
            ).read(
                urban::io::FileHandler<Lib3dsFile>(
                    arguments.input_path,
                    std::map<std::string,bool>{{"read", true}}
                )
            );
        else
        {
            scene = urban::scene::Scene(
                urban::io::FileHandler<Lib3dsFile>(
                    arguments.input_path,
                    std::map<std::string,bool>{{"read", true}}
                )
            );
        }

        std::cout << "Done." << std::flush << std::endl;

        if(arguments.prune)
            urban::prune(scene, arguments.terrain);
        
        urban::save_scene(data_directory, scene);
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
