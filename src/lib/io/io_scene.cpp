#include <io/io_scene.h>

#include <io/io_scene_tree.h>
#include <io/io_3ds.h>
#include <io/io_off.h>
#include <io/io_obj.h>

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

#include <algorithm>
#include <iterator>

namespace city
{
    namespace io
    {
        const std::vector<std::string> SceneHandler::supported_formats{{"3DS", "OFF", "OBJ"}};
        const std::vector<std::string> SceneHandler::supported_extentions{{".3ds", ".off", ".obj"}};


        SceneHandler::SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, std::string const& _format, bool const _using_xml)
            : SceneHandler(_filepath, _modes, SceneHandler::scene_format(_format), _using_xml)
        {}
        SceneHandler::SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, SceneFormat const _format, bool const _using_xml)
            : FileHandler(_filepath, _modes), format(_format), using_xml(_using_xml)
        {
            switch(format)
            {
                case t3ds:
                    check_extension();
                    break;
                case off:
                    break;
                case obj:
                    check_extension();
            }
        }
        SceneHandler::~SceneHandler(void)
        {}

        scene::Scene SceneHandler::read(void) const
        {
            scene::Scene scene;
            switch(format)
            {
                case off:
                    scene = OFFSceneHandler(filepath, using_xml).read().get_scene();
                    break;
                case obj:
                    scene = WaveObjSceneHandler(filepath, using_xml).read().get_scene();
                    break;
                case t3ds:
                    scene = T3DSSceneHandler(filepath, modes).read(using_xml);
            }
            return scene;
        }

        void SceneHandler::write(scene::Scene const& scene) const
        {
            switch(format)
            {
                case off:
                    OFFSceneHandler(filepath, scene, using_xml).write();
                    break;
                case obj:
                    WaveObjSceneHandler(filepath, scene, using_xml).write();
                    break;
                case t3ds:
                    throw std::logic_error("Not yet implemented");
            }
        }

        SceneFormat SceneHandler::scene_format(std::string const& output_format)
        {
            auto found = std::find(
                std::begin(supported_formats),
                std::end(supported_formats),
                output_format
            );
            if(found == std::end(supported_formats))
                throw std::runtime_error("Input format not supported!");
            else
                return static_cast<SceneFormat>(std::distance(std::begin(supported_formats), found));
        }
        std::string SceneHandler::extension(SceneFormat const format)
        {
            return supported_extentions.at(format);
        }

        void SceneHandler::check_extension(void) const
        {
            if(
                ! boost::iequals(
                    filepath.extension().string(),
                    SceneHandler::extension(format)
                )
            )
                throw std::runtime_error("The file path does not have the right extension");
        }
    }
}
