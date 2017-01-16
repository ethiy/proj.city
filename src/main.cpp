#include "ShadowMesh/shadow_mesh.h"
#include "UrbanObject/brick.h"
#include "IO/io.h"
#include "IO/io_3ds.h"
#include "IO/io_off.h"

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW

#include <boost/filesystem.hpp>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>

int main(int, char **)
{
    try
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string, bool> modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);
        std::vector<urban::ShadowMesh> meshes = handler.read();

        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::ShadowMesh>(std::cout, "\n"));

        std::vector<urban::Brick> urban_objects;
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::back_inserter(urban_objects),
            [](urban::ShadowMesh & mesh)
            {
                return urban::Brick(mesh);
            }
        );

        std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::Brick>(std::cout, "\n"));
        #ifdef CGAL_USE_GEOMVIEW
        CGAL::Geomview_stream geomview_stream;
        std::for_each(
            std::begin(urban_objects),
            std::end(urban_objects),
            [&](urban::Brick & obj)
            {
                geomview_stream << obj;
            });

        geomview_stream.look_recenter();
        #endif // CGAL_USE_GEOMVIEW
        std::cout << "Enter any character to stop the program:" << std::endl;
        getchar();
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
