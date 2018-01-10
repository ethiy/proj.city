#include <io/io_obj.h>

#include <boost/filesystem/operations.hpp>

#include <fstream>

namespace urban
{
    namespace io
    {
        FileHandler<Obj_stream>::FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : filepath(_filepath), modes(_modes) {}

        FileHandler<Obj_stream>::~FileHandler(void) {}

        std::vector<shadow::Mesh> FileHandler<Obj_stream>::read(void)
        {
            std::ostringstream error_message;

            std::vector<shadow::Mesh> objects;

            if (modes["read"])
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
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return objects;
        }
        void FileHandler<Obj_stream>::write(std::vector<shadow::Mesh> const& meshes)
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
    }
}
