#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <scene/scene.h>

namespace city
{
    namespace io
    {
        class OFFHandler: protected FileHandler
        {
        public:
            OFFHandler(boost::filesystem::path const& _filepath);
            OFFHandler(boost::filesystem::path const& _filepath, shadow::Mesh const& _mesh);
            OFFHandler(boost::filesystem::path const& _filepath, scene::UNode const& unode);
            ~OFFHandler(void);

            shadow::Mesh const& data(void) const noexcept;
            
            OFFHandler& read(void);
            void write(void);
        private:
            shadow::Mesh mesh;
        };

        class OFFSceneHandler: protected FileHandler
        {
        public:
            OFFSceneHandler(boost::filesystem::path const& _filepath, bool const _using_xml=true);
            OFFSceneHandler(boost::filesystem::path const& _filepath, std::vector<shadow::Mesh> const& _meshes, bool const _using_xml=true);
            OFFSceneHandler(boost::filesystem::path const& _filepath, std::vector<scene::UNode> const& unodes, bool const _using_xml=true);
            OFFSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene, bool const _using_xml=true);
            ~OFFSceneHandler(void);

            std::vector<shadow::Mesh> const& data(void) const noexcept;
            
            OFFSceneHandler& read(void);
            void write(void);

        private:
            std::vector<shadow::Mesh> meshes;
            bool using_xml;

            shadow::Point pivot;
            shadow::Bbox bbox;
            unsigned short epsg_index = 2154;
            std::vector<std::string> building_ids;
            std::string terrain_id = "terrain";
        };
    }
}
