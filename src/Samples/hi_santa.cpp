#include "../ShadowMesh/shadow_mesh.h"
#include "../UrbanObject/brick.h"
#include "../IO/io.h"
#include "../IO/io_3ds.h"

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
        std::vector<urban::ShadowMesh> meshes(handler.read());

        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::ShadowMesh>(std::cout, "\n"));

        std::vector<urban::Brick> urban_objects(2);
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::begin(urban_objects),
            [](urban::ShadowMesh & mesh)
            {
                return urban::Brick(mesh);
            }
        );

        std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::Brick>(std::cout, "\n"));
        #ifdef CGAL_USE_GEOMVIEW
        CGAL::Geomview_stream geomview_stream;
        geomview_stream.set_bg_color(CGAL::Color(0, 127, 200));
        size_t pigment(1);
        size_t all(meshes.size());
        std::for_each(
            std::begin(urban_objects),
            std::end(urban_objects),
            [&](urban::Brick & obj)
            {
                geomview_stream << CGAL::Color(250 * (pigment - 1) / all, 0, 250 * pigment / all) << obj;
                pigment++;
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
