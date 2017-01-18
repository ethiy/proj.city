#include "io_off.h"

#include "Line/line.h"
#include "../UrbanObject/brick.h"

#include <stdexcept>

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
        FileHandler<std::fstream>::FileHandler(void) {}

        FileHandler<std::fstream>::FileHandler(boost::filesystem::path _filepath, std::map<std::string, bool> _modes)
            : filepath(_filepath), modes(_modes) {}

        FileHandler<std::fstream>::~FileHandler(void) {}

        ShadowMesh FileHandler<std::fstream>::read(void)
        {
            ShadowMesh mesh;
            std::ostringstream error_message;

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
                    read_lines(file, std::back_inserter(lines));
                    if (lines.empty())
                    {
                        error_message << "This file: " << filepath.string() << " is empty!";
                        throw new std::out_of_range(error_message.str());
                    }

                    /*Ignore Comments*/
                    lines.erase(
                        std::remove_if(
                            std::begin(lines),
                            std::end(lines),
                            [](const std::string line) {
                                return line.at(0) == '#' || line.empty();
                            }),
                        std::end(lines));
                    if (lines.empty())
                    {
                        error_message << "This file: " << filepath.string() << " containes only comments!";
                        throw new std::out_of_range(error_message.str());
                    }

                    /*Parsing file*/
                    if (lines[0] == "OFF")
                    {
                        if (lines.empty())
                        {
                            error_message << "This file: " << filepath.string() << " containes only the OFF header!";
                            throw new std::out_of_range(error_message.str());
                        }

                        /*Parsing sizes*/
                        std::vector<long> sizes;
                        std::istringstream _sizes(lines[1]);
                        std::copy(std::istream_iterator<size_t>(_sizes), std::istream_iterator<size_t>(), std::back_inserter(sizes));
                        if (sizes.size() != 3)
                            throw new std::range_error("Error parsing the second line! There should be 3 integers.");
                        if (sizes[2] != 0 || sizes[0] < 0 || sizes[1] < 0)
                            throw new std::range_error("Error parsing the second line! The first and second integers sould be positive and the third is always equal to 0.");
                        if (static_cast<long>(lines.size()) != (2 + sizes[0] + sizes[1]))
                            throw new std::range_error("Error parsing the second line! The file should exactly contain the header, the sizes, the points and the faces: no more and no less.");

                        /*Parsing vertex points*/
                        std::vector<std::string> buffer_lines;
                        std::copy(std::next(std::begin(lines), 2), std::next(std::begin(lines), 2 + sizes[0]), std::back_inserter(buffer_lines));
                        size_t idx(0);

                        std::map<size_t, urban::Point> points;
                        std::vector<double> coordinates;
                        std::istringstream sline;
                        std::for_each(
                            std::begin(buffer_lines),
                            std::end(buffer_lines),
                            [&](std::string line) {
                                sline.str(line);
                                std::copy(std::istream_iterator<double>(sline), std::istream_iterator<double>(), std::back_inserter(coordinates));
                                points[idx++] = Point(coordinates[0], coordinates[1], coordinates[2]);
                                coordinates.clear();
                                sline.clear();
                            });

                        /*Parsing faces*/
                        idx = 0;
                        buffer_lines.clear();
                        sline.clear();

                        std::map<size_t, urban::Face> faces;
                        std::copy(std::next(std::begin(lines), 2 + sizes[0]), std::next(std::begin(lines), 2 + sizes[0] + sizes[1]), std::back_inserter(buffer_lines));

                        std::vector<size_t> indexes;
                        size_t n(0);
                        std::for_each(
                            std::begin(buffer_lines),
                            std::end(buffer_lines),
                            [&](std::string line) {
                                sline.str(line);
                                sline >> n;
                                std::copy(std::istream_iterator<size_t>(sline), std::istream_iterator<size_t>(), std::back_inserter(indexes));
                                if (indexes.size() != n)
                                    throw new std::range_error("Error parsing facet! The number of points parsed do not match the number of points in the line.");
                                faces[idx++] = Face(n, indexes);
                                indexes.clear();
                                sline.clear();
                            });
                        /*Mesh to return*/
                        mesh = ShadowMesh(name, points, faces);
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

        void FileHandler<std::fstream>::write(ShadowMesh mesh)
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
                     << mesh.get_number_points() << " " << mesh.get_number_faces() << " 0" << std::endl;
                
                /*Writing points*/
                std::map<size_t, Point> points = mesh.get_points();
                std::for_each(
                    std::begin(points),
                    std::end(points),
                    [&](std::pair<size_t, Point> p)
                    {
                        file << p.second << std::endl;
                    }
                );
                
                /*Writing faces*/
                std::map<size_t, Face> faces = mesh.get_faces();
                std::for_each(
                    std::begin(faces),
                    std::end(faces),
                    [&](std::pair<size_t, Face> p)
                    {
                        file << p.second << std::endl;
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
