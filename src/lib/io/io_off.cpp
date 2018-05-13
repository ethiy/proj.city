#include <io/io_off.h>

#include <io/io_scene_tree.h>

#include <io/Off_stream/off_stream.h>

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

        void OFFHandler::write(void)
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
        }

        OFFSceneHandler::OFFSceneHandler(boost::filesystem::path const& _filepath, bool const _using_xml)
            : FileHandler(_filepath, std::map<std::string, bool>{{"read", true}}), using_xml(_using_xml)
        {}
        OFFSceneHandler::OFFSceneHandler(boost::filesystem::path const& _filepath, std::vector<shadow::Mesh> const& _meshes, bool const _using_xml)
            : FileHandler(_filepath, std::map<std::string, bool>{{"write", true}}), meshes(_meshes), using_xml(_using_xml)
        {}
        OFFSceneHandler::OFFSceneHandler(boost::filesystem::path const& _filepath, std::vector<scene::UNode> const& unodes, bool const _using_xml)
            : OFFSceneHandler(_filepath, std::vector<shadow::Mesh>(unodes.size()), _using_xml)
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
        OFFSceneHandler::OFFSceneHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene, bool const _using_xml)
            : OFFSceneHandler(_filepath, scene.all_nodes(), _using_xml),
              pivot(scene.get_pivot()),
              bbox(scene.bbox()),
              building_ids(scene.identifiers()),
              terrain_id(scene.get_terrain().get_name())
        {}
        OFFSceneHandler::~OFFSceneHandler(void)
        {}

        std::vector<shadow::Mesh> const& OFFSceneHandler::data(void) const noexcept
        {
            return meshes;
        }

        OFFSceneHandler& OFFSceneHandler::read(void)
        {
            if(! boost::filesystem::is_directory(filepath))
                throw std::runtime_error("Path is not a directory");
            meshes.reserve(
                static_cast<std::size_t>(
                    std::distance(
                        boost::filesystem::directory_iterator(filepath),
                        boost::filesystem::directory_iterator()
                    )
                )
            );
            return *this;
        }
        void OFFSceneHandler::write(void)
        {
            if(!boost::filesystem::exists(filepath))
                boost::filesystem::create_directory(filepath);
            else
                if(!boost::filesystem::is_directory(filepath))
                    throw std::runtime_error("Path exists and does not correpond to a directory!");
            for(auto const& mesh : meshes)
                OFFHandler(
                    filepath / (mesh.get_name() + ".off"),
                    mesh
                ).write();
            if(using_xml)
                SceneTreeHandler(
                    filepath.parent_path() / (filepath.stem().string() + ".XML"),
                    modes
                ).write(
                    pivot,
                    bbox,
                    epsg_index,
                    building_ids,
                    terrain_id
                );
        }
    }
}
