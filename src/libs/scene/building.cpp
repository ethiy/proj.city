#include "building.h"

#include "../algorithms/brick/brick_algorithms.h"

#include <iterator>
#include <algorithm>

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
                    return Brick(mesh, pivot);
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

        std::size_t Building::size(void) const noexcept
        {
            return bricks.size();
        }

        Building::iterator Building::begin(void) noexcept
        {
            return bricks.begin();
        }
        Building::const_iterator Building::begin(void) const noexcept
        {
            return bricks.begin();
        }
        Building::const_iterator Building::cbegin(void) const noexcept
        {
            return bricks.cbegin();
        }
        Building::iterator Building::end(void) noexcept
        {
            return bricks.end();
        }
        Building::const_iterator Building::end(void) const noexcept
        {
            return bricks.end();
        }
        Building::const_iterator Building::cend(void) const noexcept
        {
            return bricks.cend();
        }

        void swap(Building & lhs, Building & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
