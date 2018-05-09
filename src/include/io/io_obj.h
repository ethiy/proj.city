#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <scene/scene.h>

#include <boost/optional.hpp>


namespace city
{
    namespace io
    {
        class WaveObjHandler: protected FileHandler
        {
        public:
            WaveObjHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            WaveObjHandler(boost::filesystem::path const& _filepath, std::vector<shadow::Mesh> const& _meshes);
            WaveObjHandler(boost::filesystem::path const& _filepath, std::vector<scene::UNode> const& unodes);
            ~WaveObjHandler(void);

            std::vector<shadow::Mesh> const& data(void) const noexcept;
            
            WaveObjHandler& read(void);

            shadow::Mesh exclude_mesh(std::string const& excluded);
            boost::optional<shadow::Mesh> mesh(std::string const& id);
            void add_mesh(shadow::Mesh const& mesh);

            void write(void);

            scene::Scene get_scene(void);
        private:
            std::vector<shadow::Mesh> meshes;
        };

        class WaveObjSceneHandler: protected WaveObjHandler
        {
        public:
            WaveObjSceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            WaveObjSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene);
            ~WaveObjSceneHandler();

            scene::Scene read(bool const using_xml = true);

            void write(bool const with_xml = true);
        private:
            boost::filesystem::path scene_tree_path;

            shadow::Point pivot;
            shadow::Bbox bbox;
            unsigned short epsg_index;
            std::vector<std::string> building_ids;
            std::string terrain_id;
        };
    }
}
