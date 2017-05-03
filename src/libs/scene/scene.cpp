#include "scene.h"

#include "../algorithms/utils/util_algorithms.h"

namespace urban
{
    /** 
     * Order a Vector of meshes by name
     * @param meshes vector of meshes.
     * @retrurn a map of name order meshes
     */
    std::map<std::string, urban::shadow::Mesh> order(std::vector<urban::shadow::Mesh> const& meshes);

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

        shadow::Point Scene::get_pivot(void) const noexcept
        {
            return pivot;
        }
        
        unsigned short Scene::get_epsg(void) const noexcept
        {
            return epsg_code;
        }

        std::map<std::size_t, std::vector<urban::shadow::Mesh> > Scene::cluster(std::vector<shadow::Mesh> const& meshes) const
        {
            std::map<std::size_t, std::vector<urban::shadow::Mesh> > buildings;

            auto ordered_meshes = order(meshes);
            std::vector<urban::shadow::Mesh> buffer_meshes;

            for(auto const& building : structure)
            {
                buffer_meshes.reserve(building.second.size());
                for(auto const& mesh_name : building.second)
                {
                    auto placeholder = ordered_meshes.find(mesh_name);
                    if(placeholder != std::end(ordered_meshes))
                        buffer_meshes.push_back(placeholder->second);
                }
                buildings.emplace(building.first, buffer_meshes);
                buffer_meshes.clear();
            }
            return buildings;
        }

        void swap(Scene & lhs, Scene & rhs)
        {
            lhs.swap(rhs);
        }
    }

    std::map<std::string, urban::shadow::Mesh> order(std::vector<urban::shadow::Mesh> const& meshes)
    {
        std::map<std::string, urban::shadow::Mesh> ordered_meshes;
        for(auto const& mesh : meshes)
        {
            ordered_meshes.emplace(mesh.get_name(), mesh);
        }
        return ordered_meshes;
    }

}