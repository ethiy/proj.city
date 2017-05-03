#include "building.h"

#include "../algorithms/brick/brick_algorithms.h"

#include <iterator>
#include <algorithm>
#include <numeric>

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

        std::string Building::get_name(void) const noexcept
        {
            std::stringstream _name(bricks.front().get_name());
            std::for_each(
                std::next(std::begin(bricks)),
                std::end(bricks),
                [&_name](Brick const& brick)
                {
                    _name << "_" << brick.get_name();
                }
            );
            return _name.str();
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

        io::Adjacency_stream & operator<<(io::Adjacency_stream & as, Building const& building)
        {
            std::vector<std::size_t> cummulative_sizes(building.size() + 1, 0);
            std::transform(
                std::begin(building.bricks),
                std::end(building.bricks),
                std::next(std::begin(cummulative_sizes)),
                [&as](Brick const& brick)
                {
                    as << brick.facets_size() << " ";
                    return brick.facets_size();
                }
            );
            as << std::endl;

            std::partial_sum(
                std::begin(cummulative_sizes),
                std::end(cummulative_sizes),
                std::begin(cummulative_sizes)
            );

            std::vector<bool> dual_matrix(cummulative_sizes.back() * cummulative_sizes.back(), false);

            std::size_t brick_index(0);
            for(auto const& brick : building.bricks)
            {
                std::for_each(
                    brick.facets_cbegin(),
                    brick.facets_cend(),
                    [&as, &brick](Brick::Facet const& facet)
                    {
                        as << facet.facet_degree() << " " << brick.area(facet) << " " << brick.centroid(facet) << " " << brick.normal(facet) << std::endl;
                    }
                );

                dual_matrix = brick.facet_adjacency_matrix(dual_matrix, cummulative_sizes.at(brick_index++));
            }

            as << dual_matrix << std::endl;

            return as;
        }

        void swap(Building & lhs, Building & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
