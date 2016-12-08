#include "io_off.h"

FileHandler<std::ifstream>::FileHandler(void){}

FileHandler<std::ifstream>::FileHandler(boost::filesystem::path _filepath, std::map<std::string, bool> _modes)
:filepath(_filepath), modes(_modes){}

FileHandler<std::ifstream>::~FileHandler(void){}

int FileHandler<std::ifstream>::read(std::vector<urban::Mesh> & meshes)
{
    try
    {
        if(modes["read"]==true)
        {
            if(boost::filesystem::is_regular_file(filepath))
            {
                
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                exit_code = boost::system::errc::no_such_file_or_directory;
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }
        }
        else
        {
            boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
            exit_code = boost::system::errc::io_error;
            throw boost::filesystem::filesystem_error(ec.message(), ec);
        }
    }
    catch (const boost::filesystem::filesystem_error& error)
    {
        std::cerr << "Reading " << filepath << " failed with: " << error.code().message() << std::endl;
    }
    return exit_code;
}
