#pragma once

#include <docopt.h>
#include <ostream>

struct Arguments
{
public:
    Arguments(std::map<std::string, docopt::value> const& docopt_args)
        : input_path(docopt_args.at("<scene>").asString()),
          rasterize(docopt_args.at("--rasterize").asBool()),
          buildings(docopt_args.at("--buildings").asBool()),
          graphs(docopt_args.at("--graphs").asBool()),
          labels(docopt_args.at("--labels").asBool()),
          no_sum(docopt_args.at("--no_sum").asBool()),
          prune(docopt_args.at("--prune").asBool())
    {
        std::cout << "Parsing arguments... " << std::flush;
        std::stringstream sconverter(docopt_args.at("--pixel_size").asString());
        sconverter >> pixel_size;
        std::cout << "Done." << std::flush << std::endl;
    }
    ~Arguments(void)
    {}

    boost::filesystem::path input_path;
    double pixel_size = 1;
    bool rasterize = false;
    bool buildings = false;
    bool graphs = false;
    bool labels = false;
    bool no_sum = false;
    bool prune = false;
};

inline std::ostream & operator <<(std::ostream & os, Arguments & arguments)
{
    os << "Arguments:" << std::endl
       << "  Input path: " << arguments.input_path << std::endl
       << "  Pixel size: " << arguments.pixel_size << std::endl
       << "  Rasterize: " << arguments.rasterize << std::endl
       << "  Buildings: " << arguments.buildings << std::endl
       << "  Graphs: " << arguments.graphs << std::endl
       << "  Labels: " << arguments.labels << std::endl
       << "  Sum projections: " << !arguments.no_sum << std::endl
       << "  Prune faces: " << !arguments.prune << std::endl;
    return os;
}
