#include "../libs/urban.h"

#include <boost/filesystem.hpp>
#include <thread>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>

int main(int, char **)
{
    try
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string, bool> modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);
        std::vector<urban::shadow::Mesh> meshes(handler.read());

        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::shadow::Mesh>(std::cout, "\n"));

        std::vector<urban::Brick> urban_objects(meshes.size());
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::begin(urban_objects),
            [](urban::shadow::Mesh & mesh)
            {
                return urban::Brick(mesh, urban::shadow::Point());
            }
        );

        std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::Brick>(std::cout, "\n"));

        #ifdef CGAL_USE_GEOMVIEW
        GeomviewVisualizer santa_visualizer(urban_objects);
        // std::thread geomview_thread(santa_visualizer);
        // geomview_thread.detach();
        #endif // CGAL_USE_GEOMVIEW
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
