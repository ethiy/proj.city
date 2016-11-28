
#include "UrbanObject/urban_object.h"
#include "IO/io.h"
#include "IO/io_3ds.h"

#include <vector>
#include <iterator>

#include <boost/filesystem.hpp>

int main(int, char**)
{
    boost::filesystem::path filepath("/home/ethiy/Workspace/GitHub/3DSceneModel/ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
    std::vector<std::string> flags;
    Reader<Lib3dsFile> reader(filepath, flags);
    Lib3dsMesh* meshes_p = reader.get_meshes();
    std::vector<UrbanObject> urban_objects;
    while(meshes_p)
    {
        urban_objects.push_back( UrbanObject(*meshes_p));
        meshes_p = meshes_p->next;
    }
    return EXIT_SUCCESS;
}
