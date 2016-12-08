#include "io_off.h"

#include "Line/line.h"

#include <cassert>

namespace urban
{
    namespace io
    {
        FileHandler<std::fstream>::FileHandler(void){}

        FileHandler<std::fstream>::FileHandler(boost::filesystem::path _filepath, std::map<std::string, bool> _modes)
        :filepath(_filepath), modes(_modes){}

        FileHandler<std::fstream>::~FileHandler(void){}

        int FileHandler<std::fstream>::read(std::vector<urban::Mesh> & meshes)
        {
            try
            {
                if(modes["read"])
                {
                    if(boost::filesystem::is_regular_file(filepath))
                    {
                        if(modes["binary"])
                        {
                            file.open(filepath.string().c_str(), std::ios::in | std::ios::binary);
                            // ERROR if file is not binary
                            std::cerr << "Binary reader not yet implemented!" << std::endl;
                            assert(0);
                            file.close();
                        }
                        else
                        {
                            file.open(filepath.string().c_str(), std::ios::in);
                            // ERROR not handled if file is binary
                            
                            file.close();
                        }
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
    }
}
