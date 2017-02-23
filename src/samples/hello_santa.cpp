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

class GeomviewVisualizer
{
public:
    inline GeomviewVisualizer(void){}
    inline GeomviewVisualizer(const std::vector<urban::Brick> & _urban_objects): urban_objects(_urban_objects) {}
    inline GeomviewVisualizer(const GeomviewVisualizer & other): geomview_stream(other.geomview_stream), urban_objects(other.urban_objects) {}
    inline GeomviewVisualizer(GeomviewVisualizer && other): geomview_stream(std::move(other.geomview_stream)), urban_objects(std::move(urban_objects)) {}
    inline ~GeomviewVisualizer(void){}

    inline GeomviewVisualizer & operator=(const GeomviewVisualizer & other) noexcept
    {
        geomview_stream = other.geomview_stream;
        urban_objects = other.urban_objects;
        return *this;
    }

    inline GeomviewVisualizer & operator=(GeomviewVisualizer && other) noexcept
    {
        geomview_stream = std::move(other.geomview_stream);
        urban_objects = std::move(other.urban_objects);
        return *this;
    }

    inline void operator()(void)
    {
        geomview_stream.set_bg_color(CGAL::Color(0, 127, 200));
        size_t pigment(1);
        size_t all(urban_objects.size());
        std::for_each(
        std::begin(urban_objects),
        std::end(urban_objects),
        [&pigment, &all, this](const urban::Brick & obj)
            {
                geomview_stream << CGAL::Color(250 * (pigment - 1) / all, 0, 250 * pigment / all) << obj;
                pigment++;
            });

        geomview_stream.look_recenter();
    }
private:
    CGAL::Geomview_stream geomview_stream;
    std::vector<urban::Brick> urban_objects;
};

#endif // CGAL_USE_GEOMVIEW


int main(int, char **)
{
    try
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string, bool> modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);
        std::vector<urban::shadow::Mesh> meshes(handler.read());

        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::shadow::Mesh>(std::cout, "\n"));

        std::vector<urban::Brick> urban_objects(2);
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::begin(urban_objects),
            [](urban::shadow::Mesh & mesh)
            {
                return urban::Brick(mesh);
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
