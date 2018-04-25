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
            t3ds_xml,
            t3ds,
            off,
            obj
        };

        class SceneHandler: protected FileHandler
        {
        public:
            SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, std::string const& _format="OFF");
            SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, SceneFormat const _format=SceneFormat::off);
            ~SceneHandler(void);

            scene::Scene read(void) const;

            void write(scene::Scene const& scene) const;
            
            static SceneFormat scene_format(std::string const& output_format);
            static std::string extension(SceneFormat const format);
        private:
            SceneFormat format;

            static const std::vector<std::string> supported_formats;
            static const std::vector<std::string> supported_extentions;

            void check_extension(void) const;
        };
    }
}
