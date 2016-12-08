#include "io_off.h"

#include "Line/line.h"

#include <cassert>

#include <string>
#include <sstream>

#include <map>
#include <vector>

#include <algorithm>
#include <iterator>

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
                            std::vector<std::string> lines;
                            read_lines(file, std::back_inserter(lines));
                            assert(!lines.empty());
                            std::copy_if(std::begin(lines), std::end(lines), std::back_inserter(lines),
                                            [](const std::string line)
                                            {
                                                return line.at(0) != '#' || !line.empty();
                                            } 
                                        );
                            assert(!lines.empty());
                            if(lines[0] == "OFF")
                            {
                                assert(!lines.empty());
                                std::vector<size_t> sizes;
                                {
                                    std::istringstream _sizes(lines[1]);
                                    std::copy(std::istream_iterator<size_t>(_sizes), std::istream_iterator<size_t>(), std::begin(sizes));
                                    assert(sizes.size()==3);
                                    assert(sizes[2] == 0);
                                    assert(lines.size() == (2+sizes[0]+sizes[1])); // assuming the last empty line is not counted!!!
                                }
                                std::map<size_t, urban::Point> points;
                                std::for_each(std::begin(lines) + 2, std::begin(lines) + sizes[0],
                                                [&](std::string line)
                                                {
                                                    return;
                                                } 
                                            );
                                
                                // Faces to parse
                            }
                            else
                            {
                                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                                exit_code = boost::system::errc::no_such_file_or_directory;
                                throw boost::filesystem::filesystem_error(ec.message(), ec);
                            }
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
