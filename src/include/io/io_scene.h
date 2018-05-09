#pragma once

#include <io/io.h>

#include <scene/scene.h>

#include <tinyxml2.h>

namespace city
{
    namespace io
    {
        enum SceneFormat : std::size_t
        {
            t3ds,
            off,
            obj
        };

        class SceneHandler: protected FileHandler
        {
        public:
            SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, std::string const& _format="OFF", bool const _using_xml = true);
            SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, SceneFormat const _format=SceneFormat::off, bool const _using_xml = true);
            ~SceneHandler(void);

            scene::Scene read(void) const;

            void write(scene::Scene const& scene) const;
            
            static SceneFormat scene_format(std::string const& output_format);
            static std::string extension(SceneFormat const format);

            static const std::vector<std::string> supported_formats;
            static const std::vector<std::string> supported_extentions;
        private:
            SceneFormat format;
            bool using_xml;

            void check_extension(void) const;
        };
    }
}
