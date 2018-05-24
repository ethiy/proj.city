#include <city.h>

namespace city
{
    void print_possible_scene_formats(void)
    {
        std::cout << "Possible formats are: ";
        std::copy(
            std::begin(city::io::SceneHandler::supported_formats),
            std::prev(std::end(city::io::SceneHandler::supported_formats)),
            std::ostream_iterator<std::string>(std::cout, ", ")
        );
        std::cout << city::io::SceneHandler::supported_formats.back() << std::endl;
    }    
}
