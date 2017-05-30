#pragma once

#include <docopt.h>
#include <ostream>

struct Arguments
{
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
