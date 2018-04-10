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
            std::transform(
                std::begin(building_ids),
                std::end(building_ids),
                std::begin(buildings),
                [&mesh_file, this](std::string const& building_id)
                {
                    return UNode(building_id, pivot, epsg_index, std::set<char>{'T', 'F'}, mesh_file);
                }
            );

            terrain = UNode(terrain_id, pivot, epsg_index, std::set<char>{'M'}, mesh_file);
        }
        Scene::Scene(io::FileHandler<Lib3dsFile> const& mesh_file, urban::shadow::Point const& _pivot, bool _centered, unsigned short _epsg_index)
            :pivot(_pivot), centered(_centered), epsg_index(_epsg_index)
        {
            auto nodes = mesh_file.get_nodes(1);
            shadow::Mesh _terrain;
            std::string terrain_id("");
            for(auto const& node: nodes)
            {
                auto meshes = mesh_file.get_mesh_by_type(node, std::set<char>{'T', 'F', 'M'});
                auto mesh = meshes['T'] + meshes['F'];
                if(mesh != shadow::Mesh())
                    buildings.push_back(
                        UNode(
                            node,
                            pivot,
                            epsg_index,
                            mesh
                        )
                    );
                _terrain += meshes['M'];
                if(meshes['M'] != shadow::Mesh())
                    terrain_id += node;
            }
            terrain = UNode(terrain_id, pivot, epsg_index, _terrain);
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

        Scene & Scene::prune(bool const _terrain)
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
            if(_terrain)
                terrain = ::urban::prune(terrain);

            return *this;
        }
    }

    scene::Scene & prune(scene::Scene & scene, bool const terrain)
    {
        return scene.prune(terrain);
    }
}
