#include "../libs/io/io_3ds.h"
#include "../libs/io/io_gdal.h"
#include "../libs/algorithms/algorithms.h"
#include "../libs/brick/brick.h"
#include "../libs/projection/projection.h"

#include <docopt.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>

static const char USAGE[]=
R"(project_scene_xy.

    Usage:
      project_scene_xy <filename>
      project_scene_xy (-h | --help)
      project_scene_xy --version
    Options:
      -h --help     Show this screen.
      --version     Show version.
)";

int main(int argc, const char** argv)
{
    try
    {
        std::map<std::string, docopt::value> args = docopt::docopt(
            USAGE,
            { argv + 1, argv + argc },
            true,
            "project_scene_xy 0.0.0-dev"
        );
        for(auto const& arg : args)
        {
            std::cout << arg.first << ": " << arg.second << std::endl;
        }
    }
    catch(const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
