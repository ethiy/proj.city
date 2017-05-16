#include "io_off.h"

#include <boost/filesystem/operations.hpp>

#include <stdexcept>

#include <string>
#include <sstream>

#include <map>
#include <vector>

namespace urban
{
    namespace io
    {
        FileHandler<Off_stream>::FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : filepath(_filepath), modes(_modes)
        {
            std::ostringstream error_message;

            if(modes["read"] && modes["write"])
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                throw boost::filesystem::filesystem_error("Simultanious reading and writing accesses are forbidden", ec);
            }
            if(!modes["read"] && !modes["write"])
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                throw boost::filesystem::filesystem_error("You have to specify the access type", ec);
            }

            std::ios_base::openmode open_mode;

            if(modes["read"])
            {
                if(boost::filesystem::is_regular_file(_filepath))
                    open_mode |= std::ios::in;
                else
                {
                    error_message << "This file \"" << _filepath.string() << "\" cannot be found! You should check the file path";
                    boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            if(modes["write"])
            {
                open_mode |= std::ios::out;
            }
            if(modes["binary"])
            {
                open_mode |= std::ios::binary;
            }
            
            file = std::fstream(_filepath.string(), open_mode);
        }

        FileHandler<Off_stream>::~FileHandler(void) {}

        shadow::Mesh FileHandler<Off_stream>::read(void)
        {
            std::ostringstream error_message;
            shadow::Mesh mesh;

            if (modes["read"])
            {
                Off_stream off_s(file);
                off_s >> mesh;
                mesh.set_name(filepath.stem().string());
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return mesh;
        }

        void FileHandler<Off_stream>::write(shadow::Mesh const& mesh)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                Off_stream off_s(file);
                off_s << mesh;
            }
            else
            {
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }
    }
}
