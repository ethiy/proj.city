#include <io/io_scene.h>

#include <io/io_scene_tree.h>
#include <io/io_3ds.h>
#include <io/io_off.h>
#include <io/io_obj.h>

#include <algorithm>
#include <iterator>

namespace urban
{
    namespace io
    {
        const std::vector<std::string> SceneHandler::supported_formats{{"3DS XML", "3DS", "OFF", "OBJ"}};
        const std::vector<std::string> SceneHandler::supported_extentions{{".3DS", ".3DS", ".off", ".obj"}};

        SceneHandler::SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, SceneFormat const _format)
            : FileHandler(_filepath, _modes), format(_format)
        {}
        SceneHandler::~SceneHandler(void)
        {}

        scene::Scene SceneHandler::read(void) const
        {
            scene::Scene scene;
            switch(format)
            {
                case off:
                    break;
                case obj:
                    scene = WaveObjHandler(filepath, std::map<std::string,bool>{{"read", true}}).get_scene();
                    break;
                case t3ds_xml:
                    scene = T3DSHandler(
                        filepath,
                        std::map<std::string,bool>{{"read", true}}
                    ).get_scene(
                        SceneTreeHandler(
                            boost::filesystem::path(
                                boost::filesystem::path(filepath.parent_path())
                                /
                                (filepath.stem().string() + ".XML")
                            )
                        ),
                        true
                    );
                    break;
                case t3ds:
                    try
                    {
                        scene = T3DSHandler(
                            filepath,
                            std::map<std::string,bool>{{"read", true}}
                        ).get_scene(
                            SceneTreeHandler(
                                boost::filesystem::path(filepath.parent_path())
                                /
                                (filepath.stem().string() + ".XML")
                            ),
                            false
                        );
                    }
                    catch(std::runtime_error const& err)
                    {
                        scene = T3DSHandler(
                            filepath,
                            std::map<std::string,bool>{{"read", true}}
                        ).get_scene();
                    }
            }
            return scene;
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
    }
}
