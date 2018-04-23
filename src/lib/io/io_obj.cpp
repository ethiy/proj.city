#include <io/io_obj.h>

#include <io/Obj_stream/obj_stream.h>

#include <fstream>
#include <sstream>

namespace urban
{
    namespace io
    {
        WaveObjHandler::WaveObjHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : FileHandler(_filepath, _modes)
        {}
        WaveObjHandler::~WaveObjHandler(void) {}

        std::vector<shadow::Mesh> const& WaveObjHandler::data(void) const noexcept
        {
            return meshes;
        }

        WaveObjHandler& WaveObjHandler::read(void)
        {
            std::ostringstream error_message;

            if (modes.at("read"))
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
                error_message << std::boolalpha << "The read mode is set to:" << modes.at("read") << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
            return *this;
        }

        void WaveObjHandler::write(void) const
        {
            if (modes.at("write"))
            {
                    std::fstream obj_file(filepath.string(), std::ios::out);
                    Obj_stream object_stream(obj_file);
                    object_stream << meshes;
            }
            else
            {
                std::ostringstream error_message;
                error_message << std::boolalpha << "The write mode is set to:" << modes.at("write") << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }

        shadow::Mesh WaveObjHandler::exclude_mesh(std::string const& excluded)
        {
            std::list<shadow::Mesh> excluded_meshes;
            std::remove_copy_if(
                std::begin(meshes),
                std::end(meshes),
                std::begin(excluded_meshes),
                [excluded](shadow::Mesh const& mesh)
                {
                    return mesh.get_name() == excluded;
                }
            );
            return std::accumulate(
                std::begin(excluded_meshes),
                std::end(excluded_meshes),
                shadow::Mesh()
            ).set_name(excluded);
        }
        void WaveObjHandler::add_mesh(shadow::Mesh const& mesh)
        {
            meshes.push_back(mesh);
        }

        scene::Scene WaveObjHandler::get_scene(void)
        {
            auto terrain = read().exclude_mesh("terrain");
            return scene::Scene(
                meshes,
                terrain
            );
        }
        WaveObjHandler& WaveObjHandler::from_scene(scene::Scene const& scene)
        {
            meshes = std::vector<shadow::Mesh>(scene.size() + 1);
            std::transform(
                std::begin(scene),
                std::end(scene),
                std::begin(meshes),
                [](scene::UNode const& unode)
                {
                    return shadow::Mesh(unode.get_surface());
                }
            );
            meshes[meshes.size()] = shadow::Mesh(scene.get_terrain().get_surface());
        }
    }
}
