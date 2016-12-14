#include "io_off.h"

#include "Line/line.h"
#include "../UrbanObject/urban_object.h"

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
        FileHandler<std::fstream>::FileHandler(void) {}

        FileHandler<std::fstream>::FileHandler(boost::filesystem::path _filepath, std::map<std::string, bool> _modes)
            : filepath(_filepath), modes(_modes) {}

        FileHandler<std::fstream>::~FileHandler(void) {}

        int FileHandler<std::fstream>::read(std::vector<urban::ShadowMesh> &meshes)
        {
            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    if (modes["binary"])
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
                        lines.erase(
                            std::remove_if(
                                std::begin(lines),
                                std::end(lines),
                                [](const std::string line) {
                                    return line.at(0) == '#' || line.empty();
                                }),
                            std::end(lines));
                        assert(!lines.empty());
                        if (lines[0] == "OFF")
                        {
                            assert(!lines.empty());
                            std::vector<long> sizes;
                            {
                                std::istringstream _sizes(lines[1]);
                                std::copy(std::istream_iterator<size_t>(_sizes), std::istream_iterator<size_t>(), std::back_inserter(sizes));
                                assert(sizes.size() == 3);
                                assert(sizes[2] == 0 && sizes[0] > 0 && sizes[1] > 0);
                                assert(lines.size() == (2 + sizes[0] + sizes[1])); // assuming the last empty line is not counted!!!
                            }

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
                                    assert(indexes.size() == n);
                                    faces[idx++] = Face(n, indexes);
                                    indexes.clear();
                                    sline.clear();
                                });

                            meshes.push_back(urban::ShadowMesh(points, faces));
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

            return exit_code;
        }

        int FileHandler<std::fstream>::write(std::vector<urban::ShadowMesh> meshes)
        {
            if (modes["write"])
            {
                std::vector<UrbanObject> objs;
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
                std::for_each(std::begin(meshes), std::end(meshes), [&](urban::ShadowMesh mesh) {
                    objs.push_back(UrbanObject(mesh));
                });
                std::copy(std::begin(objs), std::end(objs), std::ostream_iterator<UrbanObject>(file, "\n"));
                file.close();
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                exit_code = boost::system::errc::io_error;
                throw boost::filesystem::filesystem_error(ec.message(), ec);
            }
            return exit_code;
        }
    }
}
