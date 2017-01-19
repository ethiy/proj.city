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


            modes["write"] = true;
            urban::io::FileHandler<Lib3dsFile> _handler("./santa.3ds", modes);
            _handler.write(meshes);
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
