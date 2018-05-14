#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <scene/scene.h>

#include <lib3ds/file.h>

#include <boost/filesystem/operations.hpp>

#include <boost/optional.hpp>

#include <deque>
#include <set>

namespace city
{
    namespace io
    {
        class T3DSHandler: protected FileHandler
        {
        public:
            T3DSHandler(boost::filesystem::path const& _filepath);
            T3DSHandler(boost::filesystem::path const& _filepath, std::vector<shadow::Mesh> const& meshes);
            ~T3DSHandler(void);

            std::vector<shadow::Mesh> get_meshes(void);

            std::vector<shadow::Mesh> level_meshes(std::size_t const level, std::set<char> const& facet_types);
            shadow::Mesh level_mesh(std::size_t const level, std::set<char> const& facet_types);
            std::vector<std::vector<shadow::Mesh> > raw_level_meshes(std::size_t const level, std::set<char> const& facet_types);

            std::vector<shadow::Mesh> node_meshes(std::string const& node_name, std::set<char> const& facet_types);
            std::map<char, std::deque<shadow::Mesh> > node_meshes_by_type(std::string const& node_name, std::set<char> const& facet_types);
            shadow::Mesh mesh(std::string const& node_name, std::set<char> const& facet_types);
            std::map<char, shadow::Mesh> mesh_by_type(std::string const& node_name, std::set<char> const& facet_types);

            std::vector<std::string> get_nodes(std::size_t const level);

            T3DSHandler& write_meshes(void);
        private:
            Lib3dsFile* file = nullptr;

            void node_meshes(Lib3dsNode* node, std::map<char, std::deque<shadow::Mesh> > & meshes, std::set<char> const& facet_types);
        };

        class T3DSSceneHandler: protected FileHandler
        {
        public:
            T3DSSceneHandler(boost::filesystem::path const& _filepath, bool const using_xml = true);
            T3DSSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& _scene, bool const using_xml = true);
            ~T3DSSceneHandler(void);

            scene::Scene const& get_scene(void) const noexcept;

            T3DSSceneHandler& read(void);
        private:
            scene::Scene scene;
            bool using_xml;
        };
    }
}
