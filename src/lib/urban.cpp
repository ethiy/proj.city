#include <urban.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <stdexcept>

const std::vector<std::string> urban::io::supported_scene_formats{{"3DS"}};

std::string urban::io::format(std::string const& input_format)
{
    auto found = std::find_if(
        std::begin(urban::io::supported_scene_formats),
        std::end(urban::io::supported_scene_formats),
        [input_format](std::string const& frmt)
        {
            return boost::iequals(input_format, frmt);
        }
    );
    if(found == std::end(urban::io::supported_scene_formats))
        throw std::runtime_error("Input format not supported!");
    else
        return *found;
}
