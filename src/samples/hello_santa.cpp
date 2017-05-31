#include <urban.h>

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
        std::vector<urban::shadow::Mesh> meshes(handler.read());

        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::shadow::Mesh>(std::cout, "\n"));

        std::vector<urban::scene::Brick> urban_objects(meshes.size());
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::begin(urban_objects),
            [](urban::shadow::Mesh & mesh)
            {
                return urban::scene::Brick(mesh, urban::shadow::Point());
            }
        );

        std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::scene::Brick>(std::cout, "\n"));
        #ifdef CGAL_USE_GEOMVIEW
            CGAL::Geomview_stream geomview_stream;
            geomview_stream.set_bg_color(CGAL::Color(0, 127, 200));
            std::size_t pigment(1);
            std::size_t all(meshes.size());
            for(auto const& obj : urban_objects)
            {
                geomview_stream << CGAL::Color(250 * (pigment - 1) / all, 0, 250 * pigment / all) << obj;
                pigment++;
            }
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
