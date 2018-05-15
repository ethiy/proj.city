#include <scene/scene.h>

#include <algorithms/util_algorithms.h>
#include <algorithms/unode_algorithms.h>

namespace city
{
    namespace scene
    {
        Scene::Scene(void)
        {}
        Scene::Scene(
            std::vector<shadow::Mesh> const& building_meshes,
            shadow::Mesh const& terrain_mesh,
            city::shadow::Point const& _pivot,
            unsigned short _epsg_index
        )
            : pivot(_pivot), epsg_index(_epsg_index), buildings(building_meshes.size())
        {
            std::transform(
                std::begin(building_meshes),
                std::end(building_meshes),
                std::begin(buildings),
                [this](shadow::Mesh const& building_mesh)
                {
                    return UNode(building_mesh, pivot, epsg_index);
                }
            );

            terrain = UNode(terrain_mesh, pivot, epsg_index);
        }
        Scene::Scene(
            std::map<std::string, std::vector<shadow::Mesh> > const& building_meshes,
            std::vector<shadow::Mesh> const& terrain_meshes,
            city::shadow::Point const& _pivot,
            unsigned short _epsg_index
        )
            : pivot(_pivot), epsg_index(_epsg_index), buildings(building_meshes.size())
        {
            std::transform(
                std::begin(building_meshes),
                std::end(building_meshes),
                std::begin(buildings),
                [this](std::pair<std::string, std::vector<shadow::Mesh> > const& name_meshes)
                {
                    return UNode(name_meshes.first, name_meshes.second, pivot, epsg_index);
                }
            );

            terrain = UNode("terrain", terrain_meshes, pivot, epsg_index);
        }
        Scene::Scene(Scene const& other)
            : pivot(other.pivot), epsg_index(other.epsg_index), buildings(other.buildings), terrain(other.terrain)
        {}
        Scene::Scene(Scene && other)
            : pivot(std::move(other.pivot)), epsg_index(std::move(other.epsg_index)), buildings(std::move(other.buildings)), terrain(std::move(other.terrain))
        {}
        Scene::~Scene(void)
        {}

        void Scene::swap(Scene & other)
        {
            using std::swap;
            swap(pivot, other.pivot);
            swap(epsg_index, other.epsg_index);
            swap(buildings, other.buildings);
            swap(terrain, other.terrain);
        }

        Scene & Scene::operator =(Scene const& other)
        {
            pivot = other.pivot;
            epsg_index = other.epsg_index;
            buildings = other.buildings;
            terrain = other.terrain;
            
            return *this;
        }
        Scene & Scene::operator =(Scene && other)
        {
            pivot= std::move(other.pivot);
            epsg_index= std::move(other.epsg_index);
            buildings= std::move(other.buildings);
            terrain= std::move(other.terrain);
            
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
        UNode const& Scene::get_terrain(void) const noexcept
        {
            return terrain;
        }
        std::vector<UNode> const& Scene::all_buildings(void) const noexcept
        {
            return buildings;
        }
        Bbox_3 Scene::bbox(void) const
        {
            return std::accumulate(
                std::begin(buildings),
                std::end(buildings),
                terrain.bbox(),
                [](Bbox_3 const& bb, scene::UNode const& building)
                {
                    return bb + building.bbox();
                }
            );
        }
        std::vector<std::string> Scene::get_identifiers(void) const
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


        Scene::iterator Scene::begin(void) noexcept
        {
            return buildings.begin();
        }
        Scene::iterator Scene::end(void) noexcept
        {
            return buildings.end();
        }
        Scene::const_iterator Scene::begin(void) const noexcept
        {
            return buildings.begin();
        }
        Scene::const_iterator Scene::end(void) const noexcept
        {
            return buildings.end();
        }
        Scene::const_iterator Scene::cbegin(void) const noexcept
        {
            return buildings.cbegin();
        }
        Scene::const_iterator Scene::cend(void) const noexcept
        {
            return buildings.cend();
        }

        std::size_t Scene::size(void) const noexcept
        {
            return buildings.size();
        }

        void swap(Scene & lhs, Scene & rhs)
        {
            lhs.swap(rhs);
        }

        Scene & Scene::prune(bool const _terrain)
        {
            std::transform(
                std::begin(buildings),
                std::end(buildings),
                std::begin(buildings),
                [](scene::UNode & building)
                {
                    return ::city::prune(building);
                }
            );
            if(_terrain)
                terrain = ::city::prune(terrain);

            return *this;
        }
    }

    scene::Scene & prune(scene::Scene & scene, bool const terrain)
    {
        return scene.prune(terrain);
    }
}
