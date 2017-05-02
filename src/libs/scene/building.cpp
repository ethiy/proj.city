#include "building.h"

#include "../algorithms/brick/brick_algorithms.h"

namespace urban
{
    namespace scene
    {
        Building::Building(void)
        {}
        Building::Building(std::size_t const& _id, std::vector<shadow::Mesh> const& meshes, shadow::Point const& pivot)
            : id(_id), bricks(meshes.size())
        {
            std::transform(
                std::begin(meshes),
                std::end(meshes),
                std::begin(bricks),
                [&pivot](shadow::Mesh const& mesh)
                {
                    Brick brick(mesh, pivot);
                    return urban::prune(brick);
                }
            );
        }
        Building::Building(Building const& other)
            : id(other.id), bricks(other.bricks)
        {}
        Building::Building(Building && other)
            : id(std::move(other.id)), bricks(std::move(other.bricks))
        {}
        Building::~Building(void)
        {}

        void Building::swap(Building & other)
        {
            using std::swap;
            swap(id, other.id);
            swap(bricks, other.bricks);
        }

        Building & Building::operator=(Building const& other)
        {
            id = other.id;
            bricks = other.bricks;

            return *this;
        }
        Building & Building::operator=(Building && other)
        {
            id = std::move(other.id);
            bricks = std::move(other.bricks);

            return *this;
        }

        void swap(Building & lhs, Building & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
