
#include "UrbanObject/_Mesh/_mesh.h"
#include "UrbanObject/urban_object.h"
#include "IO/io.h"
#include "IO/io_3ds.h"

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

#include <boost/filesystem.hpp>

int main(int, char**)
{
    boost::filesystem::path filepath("/home/ethiy/Workspace/GitHub/3DSceneModel/ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
    std::vector<std::string> flags;
    std::vector<urban::_Mesh> meshes;

    Reader<Lib3dsFile> reader(filepath, flags);
    int exit_code = reader.get_exit_code();
    reader.get_facets(meshes);

    std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::_Mesh>(std::cout, "\n"));

    std::vector<urban::UrbanObject> urban_objects;
    std::for_each(std::begin(meshes), std::end(meshes), [&](urban::_Mesh mesh)
                                                            {
                                                                urban_objects.push_back(urban::UrbanObject(mesh));
                                                            }
                );
    std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::UrbanObject>(std::cout, "\n"));

    return exit_code;
}
