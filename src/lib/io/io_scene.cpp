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
                    if(! boost::filesystem::is_directory(filepath))
                        throw std::runtime_error("Path is not a directory");
                    {
                        std::vector<shadow::Mesh> buildings;
                        buildings.reserve(
                            static_cast<std::size_t>(
                                std::distance(
                                    boost::filesystem::directory_iterator(filepath),
                                    boost::filesystem::directory_iterator()
                                )
                            )
                        );
                        for(auto& file : boost::make_iterator_range(boost::filesystem::directory_iterator(filepath), {}))
                            if(
                                boost::filesystem::is_regular_file(file)
                                &&
                                boost::iequals(
                                    file.path().extension().string(),
                                    SceneHandler::extension(format)
                                )
                                &&
                                file.path().stem().string() != "terrain"
                            )
                                buildings.push_back(
                                    OFFHandler(file, modes).read()
                                );
                        scene = scene::Scene(
                            buildings,
                            OFFHandler(filepath / "terrain.off", modes).read()
                        );
                    }
                    break;
                case obj:
                    scene = WaveObjSceneHandler(filepath, modes).read(using_xml);
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
                    if(!boost::filesystem::exists(filepath))
                        boost::filesystem::create_directory(filepath);
                    else
                        if(!boost::filesystem::is_directory(filepath))
                            throw std::runtime_error("Path exists and does not correpond to a directory!");
                    for(auto const& building : scene)
                        OFFHandler(
                            filepath / (building.get_name() + supported_extentions.at(SceneFormat::off)),
                            modes
                        ).write(
                            shadow::Mesh(building)
                        );
                    OFFHandler(
                        filepath
                        /
                        (scene.get_terrain().get_name() + supported_extentions.at(SceneFormat::off)),
                        modes
                    ).write(
                        shadow::Mesh(scene.get_terrain())
                    );
                    break;
                case obj:
                    WaveObjSceneHandler(filepath, scene).write(using_xml);
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
