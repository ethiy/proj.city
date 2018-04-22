#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <scene/scene.h>


namespace urban
{
    namespace io
    {
        class WaveObjHandler: protected FileHandler
        {
        public:
            WaveObjHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~WaveObjHandler(void);
            
            std::vector<shadow::Mesh> read(void) const;
            void write(std::vector<shadow::Mesh> const& mesh) const;

            scene::Scene get_scene(void) const;
        };
    }
}
