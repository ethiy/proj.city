#pragma once

#include <io/io.h>

#include <projection/scene_projection.h>

#include <ogrsf_frmts.h>

#include <map>
#include <vector>
#include <string>

namespace city
{
    namespace io
    {
        class VectorHandler: protected FileHandler
        {
        public:
            VectorHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~VectorHandler(void);

            projection::FootPrint read(void);

            void write(projection::FootPrint const& vectorimage, bool const label = true);
        };

        class SceneVectorHandler: protected FileHandler
        {
        public:
            SceneVectorHandler(boost::filesystem::path const& _filepath);
            SceneVectorHandler(boost::filesystem::path const& _filepath, projection::ScenePrint const& _scene_projection);
            ~SceneVectorHandler(void);

            inline projection::ScenePrint const& get_scene(void) const
            {
                return scene_projection;
            }
            
            SceneVectorHandler& read(void);
            SceneVectorHandler const& write(bool const labels = true) const;
        private:
            projection::ScenePrint scene_projection;
        };
    }
}
