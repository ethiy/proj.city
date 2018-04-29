#include <io/io_off.h>

#include <io/Off_stream/off_stream.h>

#include <stdexcept>

#include <fstream>
#include <sstream>

namespace city
{
    namespace io
    {
        OFFHandler::OFFHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : FileHandler(_filepath, _modes)
        {}
        OFFHandler::~OFFHandler(void)
        {}

        shadow::Mesh OFFHandler::read(void)
        {
            std::ostringstream error_message;

            shadow::Mesh mesh;

            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    std::fstream off_file(filepath.string(), std::ios::in);
                    Off_stream off_stream(off_file);
                    off_stream >> mesh;
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

            return mesh;
        }

        void OFFHandler::write(shadow::Mesh const& mesh)
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
    }
}
