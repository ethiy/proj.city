#include "io_off.h"

#include "Line/line.h"
#include "../UrbanObject/urban_object.h"

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
            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    file.exceptions(std::ifstream::badbit);
                    if (modes["binary"])
                    {
                        file.open(filepath.string().c_str(), std::ios::in | std::ios::binary);
                        std::cerr << "Binary reader not yet implemented!" << std::endl;
                        file.close();
                    }
                    else
                    {
                        file.open(filepath.string().c_str(), std::ios::in);
<<<<<<< HEAD
=======
                        std::string name = filepath.stem().string();
>>>>>>> tests
                        std::vector<std::string> lines;
                        read_lines(file, std::back_inserter(lines));
                        if(lines.empty())
                            throw new std::out_of_range("The file is empty!");
                        lines.erase(
                            std::remove_if(
                                std::begin(lines),
                                std::end(lines),
                                [](const std::string line) {
                                    return line.at(0) == '#' || line.empty();
                                }),
                            std::end(lines));
                        if(lines.empty())
                            throw new std::out_of_range("The file containes only comments!");
                        if (lines[0] == "OFF")
                        {
                            if(lines.empty())
                                throw new std::out_of_range("The file containes only the OFF header!");
                            
                            std::vector<long> sizes;
                            std::istringstream _sizes(lines[1]);
                            std::copy(std::istream_iterator<size_t>(_sizes), std::istream_iterator<size_t>(), std::back_inserter(sizes));
                            if(sizes.size() != 3)
                                throw new std::range_error("Error parsing the second line!");
                            if(sizes[2] != 0 || sizes[0] < 0 || sizes[1] < 0)
                                throw new std::range_error("Error parsing the second line!");
                            if(static_cast<long>(lines.size()) != (2 + sizes[0] + sizes[1]))
                                throw new std::range_error("Error parsing the second line!");

                            std::vector<std::string> buffer_lines;
                            std::copy(std::next(std::begin(lines), 2), std::next(std::begin(lines), 2 + sizes[0]), std::back_inserter(buffer_lines));
                            size_t idx(0);

                            std::map<size_t, urban::Point> points;
                            std::vector<double> coordinates;
                            std::istringstream sline;
                            std::for_each(
                                std::begin(buffer_lines),
                                std::end(buffer_lines),
                                [&](std::string line)
                                {
                                    sline.str(line);
                                    std::copy(std::istream_iterator<double>(sline), std::istream_iterator<double>(), std::back_inserter(coordinates));
                                    points[idx++] = Point(coordinates[0], coordinates[1], coordinates[2]);
                                    coordinates.clear();
                                    sline.clear();
                                }
                            );

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
                                [&](std::string line)
                                {
                                    sline.str(line);
                                    sline >> n;
                                    std::copy(std::istream_iterator<size_t>(sline), std::istream_iterator<size_t>(), std::back_inserter(indexes));
                                    if(indexes.size() != n)
                                        throw new std::range_error("Error parsing facet!");
                                    faces[idx++] = Face(n, indexes);
                                    indexes.clear();
                                    sline.clear();
                                }
                            );

<<<<<<< HEAD
                            mesh = ShadowMesh(points, faces);
=======
                            mesh = ShadowMesh(name, points, faces);
>>>>>>> tests
                        }
                        else
                        {
                            boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                            throw boost::filesystem::filesystem_error("Not identified as OFF file!", ec);
                        }
                        file.close();
                    }
                }
                else
                {
                    boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(ec.message(), ec);
                }
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }

            return mesh;
        }

        void FileHandler<std::fstream>::write(ShadowMesh mesh)
        {
            if (modes["write"])
            {
                if (modes["binary"])
                {
                    file.open(filepath.string().c_str(), std::ios::out | std::ios::binary);
                    CGAL::set_binary_mode(file);
                }
                else
                {
                    if (modes["pretty"])
                    {
                        file.open(filepath.string().c_str(), std::ios::out);
                        CGAL::set_pretty_mode(file);
                    }
                    else
                    {
                        file.open(filepath.string().c_str(), std::ios::out);
                        CGAL::set_ascii_mode(file);
                    }
                }
                UrbanObject obj(mesh);
                file << obj;
                file.close();
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }
        }
    }
}
