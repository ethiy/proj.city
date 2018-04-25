#include <io/io_scene.h>

#include <io/io_scene_tree.h>
#include <io/io_3ds.h>
#include <io/io_off.h>
#include <io/io_obj.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <iterator>

namespace urban
{
    namespace io
    {
        const std::vector<std::string> SceneHandler::supported_formats{{"3DS XML", "3DS", "OFF", "OBJ"}};
        const std::vector<std::string> SceneHandler::supported_extentions{{".3ds", ".3ds", ".off", ".obj"}};


        SceneHandler::SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, std::string const& _format)
            : SceneHandler(_filepath, _modes, SceneHandler::scene_format(_format))
        {}
        SceneHandler::SceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes, SceneFormat const _format)
            : FileHandler(_filepath, _modes), format(_format)
        {
            switch(format)
            {
                case off:
                    if(boost::filesystem::exists(filepath))
                    {
                        if(! boost::filesystem::is_directory(filepath))
                            throw std::runtime_error("Path must be a directory");
                    }
                    else
                        boost::filesystem::create_directory(filepath);
                    break;
                case obj:
                    check_extension();
                    break;
                case t3ds_xml:
                    check_extension();
                    break;
                case t3ds:
                    check_extension();
            }
        }
        SceneHandler::~SceneHandler(void)
        {}

        scene::Scene SceneHandler::read(void) const
        {
            switch(format)
            {
                case off:
                    return scene::Scene();
                case obj:
                    return WaveObjHandler(filepath, std::map<std::string,bool>{{"read", true}}).get_scene();
                case t3ds_xml:
                    return T3DSHandler(
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
                case t3ds:
                    try
                    {
                        return T3DSHandler(
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
                        std::cerr << err.what() << std::endl;
                        return T3DSHandler(
                            filepath,
                            std::map<std::string,bool>{{"read", true}}
                        ).get_scene();
                    }
            }
        }

        void SceneHandler::write(scene::Scene const& scene) const
        {
            switch(format)
            {
                case off:
                    for(auto const& building : scene)
                    {
                        OFFHandler(
                            boost::filesystem::path(filepath / (building.get_name() + supported_extentions.at(SceneFormat::off))),
                            std::map<std::string, bool>{{"write", true}}
                        ).write(
                            shadow::Mesh(building.get_surface())
                        );
                    }
                    OFFHandler(
                        boost::filesystem::path(filepath / (scene.get_terrain().get_name() + supported_extentions.at(SceneFormat::off))),
                        std::map<std::string, bool>{{"write", true}}
                    ).write(
                        shadow::Mesh(scene.get_terrain().get_surface())
                    );
                    break;
                case obj:
                    WaveObjHandler(filepath, std::map<std::string,bool>{{"read", true}}).from_scene(scene).write();
                    break;
                case t3ds_xml:
                    throw std::logic_error("Not yet implemented");
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
                boost::iequals(
                    filepath.extension().string(),
                    SceneHandler::extension(format)
                )
            )
                throw std::runtime_error("The file path does not have the right extension");
        }
    }
}
