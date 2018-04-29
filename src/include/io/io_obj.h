#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <scene/scene.h>


namespace city
{
    namespace io
    {
        class WaveObjHandler: protected FileHandler
        {
        public:
            WaveObjHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~WaveObjHandler(void);

            std::vector<shadow::Mesh> const& data(void) const noexcept;
            
            WaveObjHandler& read(void);

            shadow::Mesh exclude_mesh(std::string const& excluded);
            void add_mesh(shadow::Mesh const& mesh);

            void write(void);

            scene::Scene get_scene(void);
            WaveObjHandler& from_scene(scene::Scene const& scene);
        private:
            std::vector<shadow::Mesh> meshes;
        };
    }
}
