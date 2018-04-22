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

        std::vector<shadow::Mesh> WaveObjHandler::read(void) const
        {
            std::ostringstream error_message;

            std::vector<shadow::Mesh> objects;

            if (modes.at("read"))
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    std::fstream obj_file(filepath.string(), std::ios::in);
                    Obj_stream object_stream(obj_file);
                    object_stream >> objects;
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

            return objects;
        }

        void WaveObjHandler::write(std::vector<shadow::Mesh> const& meshes) const
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


        scene::Scene WaveObjHandler::get_scene(void) const
        {
            return scene::Scene();
        }
    }
}
