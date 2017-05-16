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
        Building::Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& _roofs, std::vector<urban::shadow::Mesh> const& _walls, shadow::Point const& _pivot, unsigned short _epsg_code)
            : id(_id), reference_point(_pivot), epsg_code(_epsg_code), roofs(_roofs.size()), walls(_walls.size())
        {
            std::transform(
                std::begin(_roofs),
                std::end(_roofs),
                std::begin(roofs),
                [&_pivot](shadow::Mesh const& mesh)
                {
                    return Brick(mesh, _pivot);
                }
            );

            std::transform(
                std::begin(_walls),
                std::end(_walls),
                std::begin(walls),
                [&_pivot](shadow::Mesh const& mesh)
                {
                    return Brick(mesh, _pivot);
                }
            );
        }
        Building::Building(Building const& other)
            : id(other.id), reference_point(other.reference_point), epsg_code(other.epsg_code), roofs(other.roofs), walls(other.walls)
        {}
        Building::Building(Building && other)
            : id(std::move(other.id)), reference_point(std::move(other.reference_point)), epsg_code(std::move(other.epsg_code)), roofs(std::move(other.roofs)), walls(std::move(other.walls))
        {}
        Building::~Building(void)
        {}

        void Building::swap(Building & other)
        {
            using std::swap;
            swap(id, other.id);
            swap(reference_point, other.reference_point);
            swap(epsg_code, other.epsg_code);
            swap(roofs, other.roofs);
            swap(walls, other.walls);
        }

        Building & Building::operator=(Building const& other)
        {
            id = other.id;
            reference_point = other.reference_point;
            epsg_code = other.epsg_code;
            roofs = other.roofs;
            walls = other.walls;

            return *this;
        }
        Building & Building::operator=(Building && other)
        {
            id = std::move(other.id);
            reference_point = std::move(other.reference_point);
            epsg_code = std::move(other.epsg_code);
            roofs = std::move(other.roofs);
            walls = std::move(other.walls);

            return *this;
        }

        shadow::Point Building::pivot(void) const noexcept
        {
            return reference_point;
        }

        unsigned short Building::get_epsg(void) const noexcept
        {
            return epsg_code;
        }

        std::size_t Building::identifier(void) const noexcept
        {
            return id;
        }

        std::string Building::get_name(void) const noexcept
        {
            std::stringstream _name(roofs.front().get_name());
            std::for_each(
                std::next(std::begin(roofs)),
                std::end(roofs),
                [&_name](Brick const& brick)
                {
                    _name << "_" << brick.get_name();
                }
            );
            return _name.str();
        }

        bool Building::is_empty(void) const noexcept
        {
            return roofs.empty() || walls.empty();
        }

        Bbox_3 Building::bbox(void) const
        {
            Bbox_3 bbox = std::accumulate(
                std::begin(roofs),
                std::end(roofs),
                Bbox_3(),
                [](Bbox_3 & building_bbox, Brick const& brick)
                {
                    return building_bbox + brick.bbox();
                }
            );

            return std::accumulate(
                std::begin(walls),
                std::end(walls),
                bbox,
                [](Bbox_3 & building_bbox, Brick const& brick)
                {
                    return building_bbox + brick.bbox();
                }
            );
        }

        std::size_t Building::roofs_size(void) const noexcept
        {
            return roofs.size();
        }
        std::size_t Building::walls_size(void) const noexcept
        {
            return walls.size();
        }
        std::size_t Building::size(void) const noexcept
        {
            return roofs_size() + walls_size();
        }

        Building::iterator Building::roofs_begin(void) noexcept
        {
            return roofs.begin();
        }
        Building::const_iterator Building::roofs_begin(void) const noexcept
        {
            return roofs.begin();
        }
        Building::const_iterator Building::roofs_cbegin(void) const noexcept
        {
            return roofs.cbegin();
        }
        Building::iterator Building::roofs_end(void) noexcept
        {
            return roofs.end();
        }
        Building::const_iterator Building::roofs_end(void) const noexcept
        {
            return roofs.end();
        }
        Building::const_iterator Building::roofs_cend(void) const noexcept
        {
            return roofs.cend();
        }

        Building::iterator Building::walls_begin(void) noexcept
        {
            return walls.begin();
        }
        Building::const_iterator Building::walls_begin(void) const noexcept
        {
            return walls.begin();
        }
        Building::const_iterator Building::walls_cbegin(void) const noexcept
        {
            return walls.cbegin();
        }
        Building::iterator Building::walls_end(void) noexcept
        {
            return walls.end();
        }
        Building::const_iterator Building::walls_end(void) const noexcept
        {
            return walls.end();
        }
        Building::const_iterator Building::walls_cend(void) const noexcept
        {
            return walls.cend();
        }

        io::Adjacency_stream & operator<<(io::Adjacency_stream & as, Building const& building)
        {
            std::vector<std::size_t> cummulative_sizes(building.roofs_size() + 1, 0);
            std::transform(
                std::begin(building.roofs),
                std::end(building.roofs),
                std::next(std::begin(cummulative_sizes)),
                [&as](Brick const& roof)
                {
                    as << roof.facets_size() << " ";
                    return roof.facets_size();
                }
            );
            as << std::endl;

            std::partial_sum(
                std::begin(cummulative_sizes),
                std::end(cummulative_sizes),
                std::begin(cummulative_sizes)
            );

            std::vector<bool> dual_matrix(cummulative_sizes.back() * cummulative_sizes.back(), false);

            std::size_t roof_index(0);
            for(auto const& roof : building.roofs)
            {
                std::for_each(
                    roof.facets_cbegin(),
                    roof.facets_cend(),
                    [&as, &roof](Brick::Facet const& facet)
                    {
                        as << facet.facet_degree() << " " << roof.area(facet) << " " << roof.centroid(facet) << " " << roof.normal(facet) << std::endl;
                    }
                );

                dual_matrix = roof.facet_adjacency_matrix(dual_matrix, cummulative_sizes.at(roof_index++));
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
