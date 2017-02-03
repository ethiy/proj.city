#include "../ShadowMesh/shadow_mesh.h"
#include "../UrbanObject/brick.h"
#include "../IO/io.h"
#include "../IO/io_3ds.h"

#include "../Projection/brick_projection.h"
#include "../Algorithms/algorithms.h"

#include <boost/filesystem.hpp>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

int main(int, char **)
{
    try
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string, bool> modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);
        std::vector<urban::ShadowMesh> meshes = handler.read();

        std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::ShadowMesh>(std::cout, "\n"));

        std::vector<urban::Brick> urban_objects;
        std::transform(
            std::begin(meshes),
            std::end(meshes),
            std::back_inserter(urban_objects),
            [](urban::ShadowMesh & mesh)
            {
                return urban::Brick(mesh);
            }
        );
        urban::Brick santa(urban_objects[0]);
        urban::BrickProjection santa_projected = project(santa);
    }
    catch (const std::exception& except)
    {
        std::cerr << except.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
