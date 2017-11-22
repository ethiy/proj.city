#include <scene/scene.h>

#include <algorithms/util_algorithms.h>
#include <algorithms/unode_algorithms.h>

namespace urban
{
    namespace scene
    {
        Scene::Scene(void)
        {}
        Scene::Scene(urban::shadow::Point const& _pivot, bool _centered, unsigned short _epsg_index, std::vector<std::string> const& building_ids, std::string const& terrain_id, io::FileHandler<Lib3dsFile> const& mesh_file)
            :pivot(_pivot), centered(_centered), epsg_index(_epsg_index), buildings(building_ids.size())
        {
            auto _p = centered ? pivot : urban::shadow::Point();

            std::transform(
                std::begin(building_ids),
                std::end(building_ids),
                std::begin(buildings),
                [&mesh_file, &_p, this](std::string const& building_id)
                {
                    return UNode(building_id, _p, epsg_index, mesh_file);
                }
            );

            terrain = UNode(terrain_id, _p, epsg_index, mesh_file);
        }
        Scene::Scene(Scene const& other)
            :pivot(other.pivot), centered(other.centered), epsg_index(other.epsg_index), buildings(other.buildings), terrain(other.terrain)
        {}
        Scene::Scene(Scene && other)
            :pivot(std::move(other.pivot)), centered(std::move(other.centered)), epsg_index(std::move(other.epsg_index)), buildings(std::move(other.buildings)), terrain(std::move(terrain))
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
            swap(terrain, other.terrain);
        }

        Scene & Scene::operator =(Scene const& other)
        {
            pivot = other.pivot;
            centered = other.centered;
            epsg_index = other.epsg_index;
            buildings = other.buildings;
            terrain = other.terrain;
            
            return *this;
        }
        Scene & Scene::operator =(Scene && other)
        {
            pivot= std::move(other.pivot);
            centered= std::move(other.centered);
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

        Scene & Scene::prune(void)
        {
            std::transform(
                std::begin(buildings),
                std::end(buildings),
                std::begin(buildings),
                [](scene::UNode & building)
                {
                    return ::urban::prune(building);
                }
            );
            // terrain = ::urban::prune(terrain);

            return *this;
        }
    }

    scene::Scene & prune(scene::Scene & scene)
    {
        return scene.prune();
    }
}
