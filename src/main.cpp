
#include "UrbanObject/Mesh/mesh.h"
#include "UrbanObject/urban_object.h"
#include "IO/io.h"
#include "IO/io_3ds.h"

#include <CGAL/IO/Geomview_stream.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

int main(int, char**)
{
    boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
    std::map<std::string,bool> modes;
    modes.insert(std::pair<std::string, bool>("read", true));
    std::vector<urban::Mesh> meshes;

    FileHandler<Lib3dsFile> handler(filepath, modes);
    int exit_code = handler.read(meshes);

    std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::Mesh>(std::cout, "\n"));

    std::vector<urban::UrbanObject> urban_objects;
    std::for_each(std::begin(meshes), std::end(meshes), [&](urban::Mesh mesh)
                                                            {
                                                                urban_objects.push_back(urban::UrbanObject(mesh));
                                                            }
                );
    std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::UrbanObject>(std::cout, "\n"));
    
    if(!exit_code)
    {
        CGAL::Geomview_stream geomview_stream;
        std::for_each(std::begin(urban_objects), std::end(urban_objects), [&](urban::UrbanObject obj)
                                                                                {
                                                                                    geomview_stream << obj;
                                                                                }
                    );
        geomview_stream.look_recenter();
        std::cout << "Enter any character to stop the program:" << std::endl;
        getchar();
    }

    return exit_code;
}
