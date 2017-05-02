#include "scene.h"

#include "../algorithms/utils/util_algorithms.h"

namespace urban
{
    namespace scene
    {
        Scene::Scene(void)
        {}
        Scene::Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, std::set<std::string> > const& _structure)
            :pivot(_pivot), epsg_code(_epsg_code), structure(_structure)
        {}
        Scene::Scene(Scene const& other)
            :pivot(other.pivot), epsg_code(other.epsg_code), structure(other.structure)
        {}
        Scene::Scene(Scene && other)
            :pivot(std::move(other.pivot)), epsg_code(std::move(other.epsg_code)), structure(std::move(other.structure))
        {}
        Scene::~Scene(void)
        {}

        void Scene::swap(Scene & other)
        {
            using std::swap;
            swap(pivot, other.pivot);
            swap(epsg_code, other.epsg_code);
            swap(structure, other.structure);
        }

        Scene & Scene::operator=(Scene const& other)
        {
            pivot = other.pivot;
            epsg_code = other.epsg_code;
            structure = other.structure;

            return *this;
        }
        Scene & Scene::operator=(Scene && other)
        {
            pivot= std::move(other.pivot);
            epsg_code= std::move(other.epsg_code);
            structure= std::move(other.structure);

            return *this;
        }

        std::map<std::size_t, std::vector<urban::shadow::Mesh> > Scene::cluster(std::vector<shadow::Mesh> const& meshes) const
        {
            std::map<std::size_t, std::vector<urban::shadow::Mesh> > buildings;

            auto ordered_meshes = order(meshes);
            std::vector<urban::shadow::Mesh> buffer_meshes;

            for(auto const& building : structure)
            {
                buffer_meshes = std::vector<urban::shadow::Mesh>(building.second.size());
                std::transform(
                    std::begin(building.second),
                    std::end(building.second),
                    std::begin(buffer_meshes),
                    [&ordered_meshes](std::string const& mesh_name)
                    {
                        auto placeholder = std::find_if(
                            std::begin(ordered_meshes),
                            std::end(ordered_meshes),
                            [&mesh_name](std::pair<std::string, urban::shadow::Mesh> const& mesh)
                            {
                                return mesh.first == mesh_name;
                            }
                        );
                        return placeholder->second;
                    }
                );
                buildings.emplace(building.first, buffer_meshes);
            }
            return buildings;
        }

        void swap(Scene & lhs, Scene & rhs)
        {
            lhs.swap(rhs);
        }
    }
}