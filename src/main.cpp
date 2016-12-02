
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
    boost::filesystem::path filepath("../3DSceneModel/ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
    std::vector<std::string> flags;
    Reader<Lib3dsFile> reader(filepath, flags);
    std::vector<urban::_Mesh> meshes;
    reader.get_facets(meshes);
    std::vector<urban::UrbanObject> urban_objects;

    std::transform(std::begin(meshes), std::end(meshes), std::begin(urban_objects), [&](urban::_Mesh mesh)
                                                                                    {
                                                                                        return urban::UrbanObject(mesh);
                                                                                    }
                );

    std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<urban::UrbanObject>(std::cout, "\n"));
    return EXIT_SUCCESS;
}
