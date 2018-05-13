#include <io/io_obj.h>

#include <io/Obj_stream/obj_stream.h>
#include <io/io_scene_tree.h>

#include <fstream>
#include <sstream>

namespace city
{
    namespace io
    {
        WaveObjHandler::WaveObjHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : FileHandler(_filepath, _modes)
        {}
        WaveObjHandler::WaveObjHandler(boost::filesystem::path const& _filepath, std::vector<shadow::Mesh> const& _meshes)
            : FileHandler(_filepath, std::map<std::string, bool>{{"write", true}}), meshes(_meshes)
        {}
        WaveObjHandler::WaveObjHandler(boost::filesystem::path const& _filepath, std::vector<scene::UNode> const& unodes)
            : WaveObjHandler(_filepath, std::vector<shadow::Mesh>(unodes.size()))
        {
            std::transform(
                std::begin(unodes),
                std::end(unodes),
                std::begin(meshes),
                [](scene::UNode const& unode)
                {
                    return shadow::Mesh(unode);
                }
            );
        }
        WaveObjHandler::~WaveObjHandler(void) {}

        std::vector<shadow::Mesh> const& WaveObjHandler::data(void) const noexcept
        {
            return meshes;
        }

        WaveObjHandler& WaveObjHandler::read(void)
        {
            std::ostringstream error_message;

            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    std::fstream obj_file(filepath.string(), std::ios::in);
                    Obj_stream object_stream(obj_file);
                    object_stream >> meshes;
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

        void WaveObjHandler::write(void)
        {
            if (modes["write"])
            {
                    std::fstream obj_file(filepath.string(), std::ios::out);
                    Obj_stream object_stream(obj_file);
                    object_stream << meshes;
            }
            else
            {
                std::ostringstream error_message;
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }

        shadow::Mesh WaveObjHandler::exclude_mesh(std::string const& excluded)
        {
            auto part = std::stable_partition(
                std::begin(meshes),
                std::end(meshes),
                [excluded](shadow::Mesh const& mesh)
                {
                    return mesh.get_name() != excluded;
                }
            );
            std::vector<shadow::Mesh> excluded_meshes(
                std::make_move_iterator(part),
                std::make_move_iterator(std::end(meshes))
            );
            meshes.erase(part, std::end(meshes));
            return std::accumulate(
                std::begin(excluded_meshes),
                std::end(excluded_meshes),
                shadow::Mesh()
            ).set_name(excluded);
        }
        boost::optional<shadow::Mesh> WaveObjHandler::mesh(std::string const& id)
        {
            auto found = std::find_if(
                std::begin(meshes),
                std::end(meshes),
                [id](shadow::Mesh const & m)
                {
                    return m.get_name() == id;
                }
            );
            if(found == std::end(meshes))
                return boost::none;
            else
                return *found;
        }
        void WaveObjHandler::add_mesh(shadow::Mesh const& mesh)
        {
            meshes.push_back(mesh);
        }


        WaveObjSceneHandler::WaveObjSceneHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : WaveObjHandler(_filepath, _modes), scene_tree_path(filepath.parent_path() / (filepath.stem().string() + ".XML"))
        {}
        WaveObjSceneHandler::WaveObjSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene)
            : WaveObjHandler(_filepath, scene.all_nodes()),
              pivot(scene.get_pivot()),
              bbox(scene.bbox()),
              building_ids(scene.identifiers()),
              terrain_id(scene.get_terrain().get_name())
        {}
        WaveObjSceneHandler::~WaveObjSceneHandler()
        {}

        scene::Scene WaveObjSceneHandler::read(bool const using_xml)
        {
            if(!boost::filesystem::is_regular_file(scene_tree_path))
            {
                if(using_xml)
                    throw std::logic_error("Cannot extract from a file: it does not exist!");
                else
                {
                    auto terrain = WaveObjHandler::read().exclude_mesh("terrain");
                    return scene::Scene(
                        data(),
                        terrain
                    );
                }
            }
            else
            {
                SceneTreeHandler scene_tree(scene_tree_path, modes);
                pivot = scene_tree.pivot();
                epsg_index = scene_tree.epsg_index();
                if(using_xml)
                {
                    building_ids = scene_tree.building_ids();
                    terrain_id = scene_tree.terrain_id();
                    std::vector<shadow::Mesh> building_meshes(building_ids.size());
                    std::transform(
                        std::begin(building_ids),
                        std::end(building_ids),
                        std::begin(building_meshes),
                        [this](std::string const& building_id)
                        {
                            return mesh(building_id).value_or(shadow::Mesh());
                        }
                    );
                    return scene::Scene(
                        building_meshes,
                        mesh(terrain_id).value_or(shadow::Mesh()),
                        pivot,
                        epsg_index
                    );
                }
                else
                {
                    auto terrain = WaveObjHandler::read().exclude_mesh("terrain");
                    return scene::Scene(
                        data(),
                        terrain,
                        pivot,
                        epsg_index
                    );
                }
            }
        }

        void WaveObjSceneHandler::write(bool const with_xml)
        {
            WaveObjHandler::write();
            if(with_xml)
                SceneTreeHandler(scene_tree_path, modes).write(pivot, bbox, epsg_index, building_ids, terrain_id);
        }
    }
}
