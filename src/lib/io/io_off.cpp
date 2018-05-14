#include <io/io_off.h>

#include <io/io_scene_tree.h>

#include <io/Off_stream/off_stream.h>

#include <boost/algorithm/string.hpp>

#include <stdexcept>

#include <fstream>
#include <sstream>

namespace city
{
    namespace io
    {
        OFFHandler::OFFHandler(boost::filesystem::path const& _filepath)
            : FileHandler(_filepath, std::map<std::string, bool>{{"read", true}})
        {}
        OFFHandler::OFFHandler(boost::filesystem::path const& _filepath, shadow::Mesh const& _mesh)
            : FileHandler(_filepath, std::map<std::string, bool>{{"write", true}}), mesh(_mesh)
        {}
        OFFHandler::OFFHandler(boost::filesystem::path const& _filepath, scene::UNode const& unode)
            : OFFHandler(_filepath, shadow::Mesh(unode))
        {}
        OFFHandler::~OFFHandler(void)
        {}

        shadow::Mesh const& OFFHandler::data(void) const noexcept
        {
            return mesh;
        }

        OFFHandler& OFFHandler::read(void)
        {
            std::ostringstream error_message;

            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    std::fstream off_file(filepath.string(), std::ios::in);
                    Off_stream off_stream(off_file);
                    off_stream >> mesh;
                    mesh.set_name(filepath.stem().string());
                }
                else
                {
                    error_message << "This file: " << filepath.string() << " cannot be found! You should check the file path.";
                    boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return *this;
        }

        OFFHandler& OFFHandler::write(void)
        {
            if (modes["write"])
            {
                    std::fstream off_file(filepath.string(), std::ios::out);
                    Off_stream off_stream(off_file);
                    off_stream << mesh;
            }
            else
            {
                std::ostringstream error_message;
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return *this;
        }

        OFFSceneHandler::OFFSceneHandler(boost::filesystem::path const& _filepath, bool const _using_xml)
            : FileHandler(_filepath, std::map<std::string, bool>{{"read", true}}), using_xml(_using_xml)
        {}
        OFFSceneHandler::OFFSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& _scene, bool const _using_xml)
            : FileHandler(_filepath, std::map<std::string, bool>{{"write", true}}),
              scene(_scene),
              using_xml(_using_xml)
        {}
        OFFSceneHandler::~OFFSceneHandler(void)
        {}

        scene::Scene const& OFFSceneHandler::get_scene(void) const
        {
            return scene;
        }

        OFFSceneHandler& OFFSceneHandler::read(void)
        {
            if(! boost::filesystem::is_directory(filepath))
                throw std::runtime_error("Path is not a directory");
            std::vector<shadow::Mesh> building_meshes;
            building_meshes.reserve(
                static_cast<std::size_t>(
                    std::distance(
                        boost::filesystem::directory_iterator(filepath),
                        boost::filesystem::directory_iterator()
                    )
                )
            );
            if(!boost::filesystem::is_regular_file(filepath / "scene_tree.XML"))
            {
                if(using_xml)
                    throw std::logic_error("Cannot extract from a file: it does not exist!");
                else
                {
                    for(auto const& file : boost::make_iterator_range(boost::filesystem::directory_iterator(filepath), {}))
                        if(
                            boost::filesystem::is_regular_file(file)
                            &&
                            boost::iequals(
                                file.path().extension().string(),
                                ".off"
                            )
                            &&
                            file.path().stem().string() != "terrain"
                        )
                            building_meshes.push_back(
                                OFFHandler(file).read().data()
                            );
                    scene = scene::Scene(
                        building_meshes,
                        OFFHandler(filepath / "terrain.off").read().data()
                    );
                }
            }
            else
            {
                SceneTreeHandler scene_tree(filepath / "scene_tree.XML");
                if(using_xml)
                {
                    auto building_ids = scene_tree.building_ids();
                    building_meshes = std::vector<shadow::Mesh>(building_ids.size());
                    std::transform(
                        std::begin(building_ids),
                        std::end(building_ids),
                        std::begin(building_meshes),
                        [this](std::string const& building_id)
                        {
                            if(boost::filesystem::is_regular_file(filepath/ (building_id + ".off")))
                                return OFFHandler(filepath/ (building_id + ".off")).read().data();
                            else
                                return shadow::Mesh();
                        }
                    );
                    scene = scene::Scene(
                        building_meshes,
                        OFFHandler(filepath / (scene_tree.terrain_id() + ".off")).read().data(),
                        scene_tree.pivot(),
                        scene_tree.epsg_index()
                    );                    
                }
                else
                {
                    for(auto const& file : boost::make_iterator_range(boost::filesystem::directory_iterator(filepath), {}))
                    if(
                        boost::filesystem::is_regular_file(file)
                        &&
                        boost::iequals(
                            file.path().extension().string(),
                            ".off"
                        )
                        &&
                        file.path().stem().string() != "terrain"
                    )
                        building_meshes.push_back(
                            OFFHandler(file).read().data()
                        );
                    scene = scene::Scene(
                        building_meshes,
                        OFFHandler(filepath / "terrain.off").read().data(),
                        scene_tree.pivot(),
                        scene_tree.epsg_index()
                    );
                }
            }
            return *this;
        }
        OFFSceneHandler const& OFFSceneHandler::write(void) const
        {
            if(!boost::filesystem::exists(filepath))
                boost::filesystem::create_directory(filepath);
            else
                if(!boost::filesystem::is_directory(filepath))
                    throw std::runtime_error("Path exists and does not correpond to a directory!");
            
            for(auto const& building : scene)
                OFFHandler(
                    filepath / (building.get_name() + ".off"),
                    shadow::Mesh(building)
                ).write();
            OFFHandler(
                filepath / (scene.get_terrain().get_name() + ".off"),
                scene.get_terrain()
            ).write();
            if(using_xml)
                SceneTreeHandler(
                    filepath / "scene_tree.XML",
                    scene
                ).write();
            return *this;
        }
    }
}
