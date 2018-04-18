#include <io/io_off.h>

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
        FileHandler<std::fstream>::FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : filepath(_filepath), modes(_modes) {}

        FileHandler<std::fstream>::~FileHandler(void) {}

        shadow::Mesh FileHandler<std::fstream>::read(void)
        {
            std::ostringstream error_message;
            shadow::Mesh mesh;

            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    file.exceptions(std::ifstream::badbit);

                    /*Opening file*/
                    if (modes["binary"])
                        file.open(filepath.string().c_str(), std::ios::in | std::ios::binary);
                    else
                        file.open(filepath.string().c_str(), std::ios::in);
                    
                    /*Mesh name*/
                    std::string name = filepath.stem().string();

                    /*Read Lines*/
                    std::vector<std::string> lines;
                    readlines(file, std::back_inserter(lines));
                    if (lines.empty())
                    {
                        error_message << "This file: " << filepath.string() << " is empty!";
                        throw std::out_of_range(error_message.str());
                    }

                    /*Ignore Comments*/
                    lines.erase(
                        std::remove_if(
                            std::begin(lines),
                            std::end(lines),
                            [](const std::string & line)
                            {
                                return line.at(0) == '#' || line.empty();
                            }
                        ),
                        std::end(lines)
                    );
                    if (lines.empty())
                    {
                        error_message << "This file: " << filepath.string() << " contains only comments!";
                        throw std::out_of_range(error_message.str());
                    }

                    /*Parsing file*/
                    if (lines[0] == "OFF")
                    {
                        if (lines.empty())
                        {
                            error_message << "This file: " << filepath.string() << " contains only the OFF header!";
                            throw std::out_of_range(error_message.str());
                        }

                        /*Parsing sizes*/
                        std::vector<long> sizes(3);
                        std::istringstream _sizes(lines[1]);
                        std::copy(std::istream_iterator<std::size_t>(_sizes), std::istream_iterator<std::size_t>(), std::begin(sizes));
                        if (sizes.size() != 3)
                            throw std::range_error("Error parsing the second line! There should be 3 integers.");
                        if (sizes[2] != 0 || sizes[0] < 0 || sizes[1] < 0)
                            throw std::range_error("Error parsing the second line! The first and second integers should be positive and the third is always equal to 0.");
                        if (static_cast<long>(lines.size()) != (2 + sizes[0] + sizes[1]))
                            throw std::range_error("Error parsing the second line! The file should exactly contain the header, the sizes, the points and the faces: no more and no less.");

                        /*Parsing vertex points*/
                        std::vector<std::string> buffer_lines(static_cast<std::size_t>(sizes[0]));
                        std::copy(std::next(std::begin(lines), 2), std::next(std::begin(lines), 2 + sizes[0]), std::begin(buffer_lines));

                        std::vector<shadow::Point> points(buffer_lines.size());
                        std::vector<double> coordinates;
                        coordinates.reserve(3);
                        std::istringstream sline;
                        std::transform(
                            std::begin(buffer_lines),
                            std::end(buffer_lines),
                            std::begin(points),
                            [&sline, &coordinates](std::string const& line)
                            {
                                coordinates.clear();
                                sline.clear();
                                sline.str(line);
                                std::copy(std::istream_iterator<double>(sline), std::istream_iterator<double>(), std::back_inserter(coordinates));
                                return shadow::Point(coordinates[0], coordinates[1], coordinates[2]);
                            }
                        );

                        /*Parsing faces*/
                        buffer_lines.clear();
                        sline.clear();

                        buffer_lines.resize(static_cast<std::size_t>(sizes[1]));
                        std::copy(std::next(std::begin(lines), 2 + sizes[0]), std::next(std::begin(lines), 2 + sizes[0] + sizes[1]), std::begin(buffer_lines));

                        std::vector<shadow::Face> faces(buffer_lines.size());

                        std::vector<std::size_t> indexes;
                        std::size_t n(0);
                        std::transform(
                            std::begin(buffer_lines),
                            std::end(buffer_lines),
                            std::begin(faces),
                            [&indexes, &sline, &n](std::string line)
                            {
                                indexes.clear();
                                sline.clear();
                                sline.str(line);
                                sline >> n;
                                indexes.resize(n);
                                std::copy(std::istream_iterator<std::size_t>(sline), std::istream_iterator<std::size_t>(), std::begin(indexes));
                                if (indexes.size() != n)
                                    throw std::range_error("Error parsing facet! The number of points parsed do not match the number of points in the line.");
                                return shadow::Face(indexes);
                            }
                        );
                        /*Mesh to return*/
                        mesh = shadow::Mesh(name, points, faces);
                    }
                    else
                    {
                        error_message << "Not identified as OFF file! OFF files starts with a \'OFF\' hearder line.";
                        boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                        throw boost::filesystem::filesystem_error(error_message.str(), ec);
                    }

                    /*Closing file*/
                    file.close();
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

        void FileHandler<std::fstream>::write(shadow::Mesh const& mesh)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                if (modes["binary"])
                {
                    file.open(filepath.string().c_str(), std::ios::out | std::ios::binary);
                }
                else
                {
                    file.open(filepath.string().c_str(), std::ios::out);
                }

                /*Writing comments, header and sizes*/
                file << "# Mesh: " << mesh.get_name() << std::endl
                     << "OFF" << std::endl
                     << mesh.points_size() << " " << mesh.faces_size() << " 0" << std::endl;
                
                /*Writing points*/
                std::for_each(
                    mesh.points_cbegin(),
                    mesh.points_cend(),
                    [this](shadow::Point const& point)
                    {
                        file << point << std::endl;
                    }
                );
                
                /*Writing faces*/
                std::for_each(
                    mesh.faces_cbegin(),
                    mesh.faces_cend(),
                    [this](shadow::Face const& face)
                    {
                        file << face << std::endl;
                    }
                );

                file.close();
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
