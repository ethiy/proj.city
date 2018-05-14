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
            WaveObjHandler(boost::filesystem::path const& _filepath);
            WaveObjHandler(boost::filesystem::path const& _filepath, std::vector<shadow::Mesh> const& _meshes);
            WaveObjHandler(boost::filesystem::path const& _filepath, std::vector<scene::UNode> const& unodes);
            ~WaveObjHandler(void);

            std::vector<shadow::Mesh> const& get_meshes(void) const noexcept;
            
            WaveObjHandler& read(void);

            shadow::Mesh exclude_mesh(std::string const& excluded);
            boost::optional<shadow::Mesh> mesh(std::string const& id) const;

            WaveObjHandler& write(void);
        private:
            std::vector<shadow::Mesh> meshes;
        };

        class WaveObjSceneHandler: protected FileHandler
        {
        public:
            WaveObjSceneHandler(boost::filesystem::path const& _filepath, bool const _using_xml = true);
            WaveObjSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene, bool const _using_xml = true);
            ~WaveObjSceneHandler(void);

            scene::Scene const& get_scene() const;

            WaveObjSceneHandler& read(void);

            WaveObjSceneHandler const& write(void) const;
        private:
            scene::Scene scene;
            bool using_xml;
        };
    }
}
