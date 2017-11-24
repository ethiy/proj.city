#pragma once

#include <shadow/mesh.h>

#include <io/Line/line.h>

#include <ostream>

#include <stdexcept>

#include <vector>
#include <algorithm>
#include <iterator>
#include <ios>


namespace urban
{
    namespace io
    {
        /**
         * @ingroup io
         * @brief formats an output stream to the OFF
         */
        class Obj_stream
        {
        public:
            /**
            * Reference constructor
            * @param _ios reference to input/output stream
            * @see Obj_stream(std::ostream && _ios)
            * @see Obj_stream(Obj_stream & _ios)
            * @see Obj_stream(Obj_stream && _ios)
            */
            Obj_stream(std::iostream & _ios): ios(_ios) {}
            /**
            * Copy constructor
            * @param other reference to Adjacency stream
            * @see Obj_stream(std::iostream & _ios)
            * @see Obj_stream(std::iostream && _ios)
            * @see Obj_stream(Obj_stream && other)
            */
            Obj_stream(Obj_stream & other): ios(other.ios) {}
            /**
            * Defines operator<< for this stream.
            * @tparam T output value type
            * @param value value to output
            * @return reference to the Obj_stream
            */
            template<typename T>
            Obj_stream & operator <<(const T & value)
            {
                ios << value; 
                return *this;
            }

            /**
            * Defines operator>> for this stream.
            * @tparam T input value type
            * @param value value to input
            * @return reference to the Obj_stream
            */
            template<typename T>
            Obj_stream & operator >>(const T & value)
            {
                ios >> value; 
                return *this;
            }

            /**
            * Defines operator<< for this stream.
            * @param func function applied to input/output stream
            * @return reference to the Obj_stream
            */
            Obj_stream & operator <<(std::iostream& (*func)(std::iostream&) )
            {
                func(ios);
                return *this;
            }

            /** 
             * Writes Mesh to output off stream.
             * @param mesh the Mesh to write
             * @return the off output stream
             */
            Obj_stream & operator <<(shadow::Mesh const& mesh)
            {
                print_points(mesh);
                print_objects(mesh);

                return *this;
            }
            /** 
             * Reads Mesh from off stream.
             * @param mesh the Mesh to write
             * @return the output stream
             */
            Obj_stream & operator >>(std::vector<shadow::Mesh> & meshes)
            {
                auto lines = parse();

                if (lines.empty())
                    throw std::out_of_range("The stream contains only comments and/or empty lines!");


                return *this;
            }

        private:
            /** reference to a stream */
            std::iostream & ios;

            std::vector<std::string> parse(void)
            {
                std::vector<std::string> lines;
                readlines(ios, std::back_inserter(lines));

                /*Ignore Comments*/
                lines.erase(
                    std::remove_if(
                        std::begin(lines),
                        std::end(lines),
                        [](std::string const& line)
                        {
                            return line.at(0) == '#' || line.empty();
                        }
                    ),
                    std::end(lines)
                );

                return lines;
            }

            std::vector<shadow::Point> read_points(std::vector<std::string> const& lines, std::vector<std::string>::iterator & cursor)
            {
                /*
                   It is initialized by the origin:
                    - it corresponds to the pivot
                    - and `obj' point indexes starts at 1.
                */
                std::vector<shadow::Point> points(1);
                points.reserve(lines.size() / 2);

                while(cursor != std::end(lines) && *cursor[0] == 'v')
                {
                    points.push_back(
                        str2pt(cursor->substr(1))
                    );
                    ++cursor;
                }
                return points;
            }

            std:vector<shadow::Mesh> read_objects(std::vector<std::string> const& lines, std::vector<std::string>::iterator & cursor, std::vector<shadow::Point> const& points)
            {
                std::vector<std::string> objects = object_names(lines, cursor);

                std::vector<shadow::Mesh> meshes(objects.size());

                std::transform(
                    std::begin(objects),
                    std::end(objects),
                    std::begin(meshes),
                    [&lines, &points](std::string const& name)
                    {
                        return read_object(lines, points, name);
                    }
                );

                return meshes;
            }

            shadow:Mesh read_object(std::vector<std::string> const& lines, std::vector<shadow::Point> const& points, std::string const& name)
            {
                std::map<std::size_t, std::size_t> indexe_map;
                std::vector<Face> facets = read_facets(lines, name, index_map);

                return shadow::Mesh(name, filter(points, index_map), facets);
            }

            std::vector<Face> read_facets(std::vector<std::string> const& lines, std::string const& name, std::map<std::size_t, std::size_t> & index_map)
            {
                
                return ;
            }

            shadow::Face read_facet(std::string const& line, std::map<std::size_t, std::size_t> & index_map)
            {
                std::istringstream buffer_line(line);

                std::vector<std::size_t> indexes;
                indexes.reserve(4);

                std::copy(
                    std::istream_iterator<double>(buffer_line),
                    std::istream_iterator<double>(),
                    std::back_inserter(indexes)
                );

                auto new_index = index_map.size();
                for(auto index : indexes)
                    index_map.emplace(std::make_pair(index, new_index++));

                return shadow::Face(indexes);
            }

            std::vector<std::string> object_names(std::vector<std::string> const& lines, std::vector<std::string>::iterator & cursor)
            {
                std::vector<std::string> objects;
                objects.reserve(
                    static_cast<std::size_t>(
                        std::distance(std::begin(lines), cursor)
                    )
                    /
                    3
                );

                std::stringstream buffer;

                for(; cursor != std::end(lines); ++cursor)
                    if(*cursor[0] == 'o')
                    {
                        buffer.str(cursor->substr(1));
                        std::string name("");
                        buffer >> name;
                        objects.push_back(name);
                        buffer.clear();
                    }
                
                return objects;
            }
        };

        shadow::Point str2pt(std::string const& line)
        {
            std::istringstream buffer_line(line);

            std::vector<double> buffer;
            buffer.reserve(4);

            std::copy(
                std::istream_iterator<double>(buffer_line),
                std::istream_iterator<double>(),
                std::back_inserter(buffer)
            );

            return str2pt(buffer);
        }
        shadow::Point str2pt(std::vector<double> const& coordinates)
        {
            shadow::Point point;

            switch(coordinates.size())
            {
                case 3: 
                    shadow::Point(coordinates[0], coordinates[1], coordinates[2]);
                    break;
                case 4:
                    static_cast<bool>(coordinates[4])
                        ? shadow::Point(coordinates[0] / coordinates[4], coordinates[1] / coordinates[4], coordinates[2] / coordinates[4])
                        : throw std::logic_error("Not implemented in this scope!");
                    break;
                default:
                    throw std::ios_base::failure("Unable to read points!");
                    break;
            }

            return point;
        }
    }
}
