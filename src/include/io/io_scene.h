#pragma once

#include <io/io.h>

#include <scene/scene.h>

#include <tinyxml2.h>

namespace urban
{
    namespace io
    {
        enum SceneFormat : std::size_t
        {
            3ds,
            off,
            obj
        };

        class SceneHandler: FileHandler
        {
        public:
            SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, SceneFormat const _format = SceneFormat::off);
            ~SceneHandler(void);

            scene::Scene read(void);

            void write(scene::Scene const& scene);
            
            static SceneFormat format(std::string const& output_format);
            static std::string extension(SceneFormat const format);
        private:
            SceneFormat format;
            static const std::vector<std::string> supported_formats;
            static const std::vector<std::string> supported_extentions;
        };
    }
}
