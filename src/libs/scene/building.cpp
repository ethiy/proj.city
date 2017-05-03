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
        Building::Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot, unsigned short _epsg_code)
            : id(_id), reference_point(pivot), epsg_code(_epsg_code), bricks(meshes.size())
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
            : id(other.id), reference_point(other.reference_point), epsg_code(other.epsg_code), bricks(other.bricks)
        {}
        Building::Building(Building && other)
            : id(std::move(other.id)), reference_point(std::move(other.reference_point)), epsg_code(std::move(other.epsg_code)), bricks(std::move(other.bricks))
        {}
        Building::~Building(void)
        {}

        void Building::swap(Building & other)
        {
            using std::swap;
            swap(id, other.id);
            swap(bricks, other.bricks);
            swap(reference_point, other.reference_point);
            swap(epsg_code, other.epsg_code);
        }

        Building & Building::operator=(Building const& other)
        {
            id = other.id;
            bricks = other.bricks;
            reference_point = other.reference_point;
            epsg_code = other.epsg_code;

            return *this;
        }
        Building & Building::operator=(Building && other)
        {
            id = std::move(other.id);
            bricks = std::move(other.bricks);
            reference_point = std::move(other.reference_point);
            epsg_code = std::move(other.epsg_code);

            return *this;
        }

        shadow::Point Building::pivot(void) const noexcept
        {
            return reference_point;
        }

        std::size_t Building::identifier(void) const noexcept
        {
            return id;
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
