#pragma once

#include <docopt.h>
#include <ostream>

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
          rasterize(docopt_args.at("--rasterize").asBool())
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
       << "  Pixel size: " << arguments.pixel_size << std::endl;
    return os;
}
