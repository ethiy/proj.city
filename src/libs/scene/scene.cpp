#include "scene.h"

#include "../algorithms/utils/util_algorithms.h"

namespace urban
{
    namespace scene
    {
        BComposition & BComposition::operator =(BComposition const& other)
        {
            roofs = other.roofs;
            walls = other.walls;

            return *this;
        }
        BComposition & BComposition::operator =(BComposition && other)
        {
            roofs = std::move(other.roofs);
            walls = std::move(other.walls);

            return *this;
        }

        void swap(BComposition & lhs, BComposition & rhs)
        {
            std::swap(lhs.roofs, rhs.roofs);
            std::swap(lhs.walls, rhs.walls);
        }

        Scene::Scene(void)
        {}
        Scene::Scene(urban::shadow::Point const& _pivot, bool _centered, unsigned short _epsg_index, std::vector<std::string> const& building_ids, io::FileHandler<Lib3dsFile> const& mesh_file)
            :pivot(_pivot), centered(_centered), epsg_index(_epsg_index), buildings(building_ids.size())
        {
            std::transform(
                std::begin(building_ids),
                std::end(building_ids),
                std::begin(buildings),
                [&mesh_file, this](std::string const& building_id)
                {
                    if(centered)
                        return UNode(building_id, pivot, epsg_index, mesh_file);
                    else
                        return UNode(building_id, urban::shadow::Point(), epsg_index, mesh_file);
                }
            );
        }
        Scene::Scene(Scene const& other)
            :pivot(other.pivot), centered(other.centered), epsg_index(other.epsg_index), buildings(other.buildings)
        {}
        Scene::Scene(Scene && other)
            :pivot(std::move(other.pivot)), centered(std::move(other.centered)), epsg_index(std::move(other.epsg_index)), buildings(std::move(other.buildings))
        {}
        Scene::~Scene(void)
        {}

        void Scene::swap(Scene & other)
        {
            using std::swap;
            swap(pivot, other.pivot);
            swap(centered, other.centered);
            swap(epsg_index, other.epsg_index);
            swap(buildings, other.buildings);
        }

        Scene & Scene::operator =(Scene const& other)
        {
            pivot = other.pivot;
            centered = other.centered;
            epsg_index = other.epsg_index;
            buildings = other.buildings;

            return *this;
        }
        Scene & Scene::operator =(Scene && other)
        {
            pivot= std::move(other.pivot);
            centered= std::move(other.centered);
            epsg_index= std::move(other.epsg_index);
            buildings= std::move(other.buildings);

            return *this;
        }

        shadow::Point Scene::get_pivot(void) const noexcept
        {
            return pivot;
        }
        
        unsigned short Scene::get_epsg(void) const noexcept
        {
            return epsg_index;
        }

        std::vector<std::string> Scene::identifiers(void) const
        {
            std::vector<std::string> ids(buildings.size());
            std::transform(
                std::begin(buildings),
                std::end(buildings),
                std::begin(ids),
                [](UNode const& _building)
                {
                    return _building.get_name();
                }
            );
            return ids;
        }

        void swap(Scene & lhs, Scene & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
