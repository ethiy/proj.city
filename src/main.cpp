#include "ShadowMesh/shadow_mesh.h"
#include "UrbanObject/urban_object.h"
#include "IO/io.h"
#include "IO/io_3ds.h"
#include "IO/io_off.h"

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW

#include <easylogging++.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>

INITIALIZE_EASYLOGGINGPP

int main(int, char **)
{
    try
    {
        LOG(INFO) << "Start up the program ...";
        el::Configurations default_conf("../../confs/default_conf.conf");
        el::Loggers::reconfigureAllLoggers(default_conf);

        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string, bool> modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);
        LOG(INFO) << "Starting to read \"" << filepath.string() << "\" ...";
        std::vector<urban::ShadowMesh> meshes = handler.read();

        LOG(INFO) << "\""<< filepath.string() << "\” has been read.";
        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::ShadowMesh>(std::cout, "\n"));

        std::vector<urban::UrbanObject> urban_objects;
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::back_inserter(urban_objects),
            [](urban::ShadowMesh mesh)
            {
                return urban::UrbanObject(mesh);
            }
        );

        std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::UrbanObject>(std::cout, "\n"));
        #ifdef CGAL_USE_GEOMVIEW
        LOG(INFO) << "Starting up Geomview stream ...";
        CGAL::Geomview_stream geomview_stream;
        std::for_each(
            std::begin(urban_objects),
            std::end(urban_objects),
            [&](urban::UrbanObject obj)
            {
                geomview_stream << obj;
            });

        geomview_stream.look_recenter();
        #endif // CGAL_USE_GEOMVIEW
        LOG(INFO) << "Enter any character to stop the program:";
        getchar();
        LOG(INFO) << "... end of Visualization.";
        LOG(INFO) << "... end of program.";
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
