
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
    Reader<Lib3dsFile> reader(filepath, flags);
    Lib3dsMesh* meshes_p = reader.get_meshes();
    std::vector<UrbanObject> urban_objects;
    size_t counter = 1;
    while(meshes_p)
    {
        std::cout << "mesh: " << counter++ << std::endl;
        urban_objects.push_back( UrbanObject(*meshes_p));
        meshes_p = meshes_p->next;
    }
    std::copy(std::begin(urban_objects), std::end(urban_objects), std::ostream_iterator<UrbanObject>(std::cout, "\n"));
    return EXIT_SUCCESS;
}
