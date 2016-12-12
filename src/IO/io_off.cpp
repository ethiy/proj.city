#include "io_off.h"

#include "Line/line.h"
#include "../UrbanObject/urban_object.h"

#include <boost/range/combine.hpp>
#include <boost/foreach.hpp>

#include <cassert>

#include <string>
#include <sstream>

#include <map>
#include <vector>
#include <array>

#include <algorithm>
#include <iterator>
#include <numeric>

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
                                {
                                    std::vector<size_t> indexes(sizes[0]);
                                    std::iota(std::begin(indexes), std::end(indexes), 0);
                                    std::vector<std::string> point_lines(sizes[0]);
                                    std::copy(std::next(std::begin(lines), 2), std::next(std::begin(lines) , 2 + sizes[0]), std::begin(point_lines));
                                    
                                    size_t idx;
                                    std::string point_line;
                                    std::array<double, 3> coordinates;
                                    BOOST_FOREACH(boost::tie(idx, point_line), boost::combine(indexes, point_lines))
                                    {
                                        std::istringstream _point_line(point_line);
                                        std::copy(std::istream_iterator<double>(_point_line), std::istream_iterator<double>(), std::begin(coordinates));
                                        points[idx] = Point(coordinates[0], coordinates[1], coordinates[2]);
                                    }
                                }
                                std::map<size_t, urban::Face> faces;
                                {
                                    std::vector<size_t> indexes(sizes[1]);
                                    std::iota(std::begin(indexes), std::end(indexes), 0);
                                    std::vector<std::string> face_lines(sizes[1]);
                                    std::copy(std::next(std::begin(lines), 2 + sizes[0] ), std::next(std::begin(lines) , 2 + sizes[0] + sizes[1]), std::begin(face_lines));
                                    
                                    size_t idx;
                                    std::string face_line;
                                    std::vector<size_t> index_line, corners;
                                    BOOST_FOREACH(boost::tie(idx, face_line), boost::combine(indexes, face_lines))
                                    {
                                        std::istringstream _face_line(face_line);
                                        std::copy(std::istream_iterator<size_t>(_face_line), std::istream_iterator<size_t>(), std::begin(index_line));
                                        assert(index_line.size() == 1 + index_line[0]);
                                        std::copy(std::next(std::begin(index_line), 1), std::end(index_line), std::begin(corners));
                                        faces[idx] = Face(index_line[0], corners); // to correct of course
                                    }
                                }
                                meshes.push_back(urban::Mesh(points, faces));
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

        int FileHandler<std::fstream>::write(std::vector<urban::Mesh> meshes)
        {
            try
            {
                if(modes["write"])
                {
                    std::vector<UrbanObject> objs;
                    if(modes["binary"])
                    {
                        file.open(filepath.string().c_str(), std::ios::out | std::ios::binary);
                        CGAL::set_binary_mode(file);
                    }
                    else
                    {
                        if(modes["pretty"])
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
                    std::for_each(std::begin(meshes), std::end(meshes), [&](urban::Mesh mesh)
                                                                            {
                                                                                objs.push_back(UrbanObject(mesh));
                                                                            }
                                );
                    std::copy(std::begin(objs), std::end(objs), std::ostream_iterator<UrbanObject>(file, "\n"));
                    file.close();
                }
                else
                {
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    exit_code = boost::system::errc::io_error;
                    throw boost::filesystem::filesystem_error(ec.message(), ec);
                }
            }
            catch (const boost::filesystem::filesystem_error & error)
            {
                std::cerr << "Writing " << filepath << " failed with: " << error.code().message() << std::endl;
            }
            return exit_code;
        }
    }
}
