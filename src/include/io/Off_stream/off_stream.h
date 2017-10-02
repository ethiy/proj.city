#pragma once

#include <shadow/mesh.h>

#include <io/Line/line.h>

#include <ostream>

#include <stdexcept>

#include <vector>
#include <algorithm>
#include <iterator>


namespace urban
{
    namespace io
    {
        /**
         * @ingroup io
         * @brief formats an output stream to the OFF
         */
        class Off_stream
        {
        public:
            /**
            * Reference constructor
            * @param _ios reference to input/output stream
            * @see Off_stream(std::ostream && _ios)
            * @see Off_stream(Off_stream & _ios)
            * @see Off_stream(Off_stream && _ios)
            */
            Off_stream(std::iostream & _ios): ios(_ios) {}
            /**
            * Copy constructor
            * @param other reference to Adjacency stream
            * @see Off_stream(std::iostream & _ios)
            * @see Off_stream(std::iostream && _ios)
            * @see Off_stream(Off_stream && other)
            */
            Off_stream(Off_stream & other): ios(other.ios) {}
            /**
            * Defines operator<< for this stream.
            * @tparam T output value type
            * @param value value to output
            * @return reference to the Off_stream
            */
            template<typename T>
            Off_stream & operator <<(const T & value)
            {
                ios << value; 
                return *this;
            }

            /**
            * Defines operator>> for this stream.
            * @tparam T input value type
            * @param value value to input
            * @return reference to the Off_stream
            */
            template<typename T>
            Off_stream & operator >>(const T & value)
            {
                ios >> value; 
                return *this;
            }

            /**
            * Defines operator<< for this stream.
            * @param func function applied to input/output stream
            * @return reference to the Off_stream
            */
            Off_stream & operator <<(std::iostream& (*func)(std::iostream&) )
            {
                func(ios);
                return *this;
            }

            /** 
             * Writes Mesh to output off stream.
             * @param mesh the Mesh to write
             * @return the off output stream
             */
            Off_stream & operator <<(shadow::Mesh const& mesh)
            {
                print_header(mesh);
                print_points(mesh);
                print_faces(mesh);

                return *this;
            }
            /** 
             * Reads Mesh from off stream.
             * @param mesh the Mesh to write
             * @return the output stream
             */
            Off_stream & operator >>(shadow::Mesh & mesh)
            {
                std::stringstream error_message;
                
                /*Read Lines*/
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
                if (lines.empty())
                    throw std::out_of_range("The stream contains only comments and/or empty lines!");

                /*Parsing file*/
                if (lines.at(0) == "OFF")
                {
                    if (lines.size() <= 1)
                        throw std::out_of_range("The stream contains only the header; nothing to parse!");

                    auto sizes = read_header(lines);

                    mesh = shadow::Mesh(
                        read_points(
                            lines,
                            sizes[0]
                        ),
                        read_faces(
                            lines,
                            2 + sizes[0],
                            sizes[1]
                        )
                    );
                }
                else
                    throw std::ios_base::failure("Not identified as OFF format! OFF files starts with a \'OFF\' hearder line.");
                return *this;
            }

        private:
            /** reference to a stream */
            std::iostream & ios;

            void print_header(shadow::Mesh const& mesh)
            {
                ios << "# Mesh: " << mesh.get_name() << std::endl
                    << "OFF" << std::endl
                    << mesh.points_size() << " " << mesh.faces_size() << " 0" << std::endl;
            }
            void print_points(shadow::Mesh const& mesh)
            {
                std::for_each(
                    mesh.points_cbegin(),
                    mesh.points_cend(),
                    [this](shadow::Point const& points)
                    {
                        ios << points << std::endl;
                    }
                );
            }
            void print_faces(shadow::Mesh const& mesh)
            {
                std::for_each(
                    mesh.faces_cbegin(),
                    mesh.faces_cend(),
                    [this](shadow::Face const& facets)
                    {
                        ios << facets << std::endl;
                    }
                );
            }

            std::vector<std::size_t> read_header(std::vector<std::string> const& lines)
            {
                std::vector<std::size_t> sizes(3);
                std::stringstream _sizes(lines[1]);
                std::copy(
                    std::istream_iterator<std::size_t>(_sizes),
                    std::istream_iterator<std::size_t>(),
                    std::begin(sizes)
                );
                if(sizes.size() != 3)
                    throw std::range_error("Error parsing the second line! There should be 3 integers.");
                if(sizes[2] != 0 || sizes[0] < 0 || sizes[1] < 0)
                    throw std::range_error("Error parsing the second line! The first and second integers should be positive and the third is always equal to 0.");
                if(lines.size() != (2 + sizes[0] + sizes[1]))
                    throw std::range_error("Error parsing the second line! The file should exactly contain the header, the sizes, the points and the faces: no more and no less.");

                return sizes;
            }
            std::vector<shadow::Point> read_points(std::vector<std::string> const& lines, std::size_t const number_of_points)
            {
                std::vector<std::string> buffer_lines(static_cast<std::size_t>(number_of_points));
                std::copy(
                    std::next(std::begin(lines), 2),
                    std::next(std::begin(lines), 2 + static_cast<long>(number_of_points)),
                    std::begin(buffer_lines)
                );

                std::vector<shadow::Point> points(buffer_lines.size());
                std::vector<double> coordinates;
                coordinates.reserve(3);
                std::istringstream sline;
                std::transform(
                    std::begin(buffer_lines),
                    std::end(buffer_lines),
                    std::begin(points),
                    [&points, &sline, &coordinates](std::string const& line)
                    {
                        coordinates.clear();
                        sline.clear();
                        sline.str(line);
                        std::copy(std::istream_iterator<double>(sline), std::istream_iterator<double>(), std::back_inserter(coordinates));
                        return shadow::Point(coordinates[0], coordinates[1], coordinates[2]);
                    }
                );

                return points;
            }
            std::vector<shadow::Face> read_faces(std::vector<std::string> const& lines, std::size_t const shift, std::size_t const number_of_faces)
            {
                std::vector<std::string> buffer_lines(number_of_faces);
                std::copy(
                    std::next(std::begin(lines), static_cast<long>(shift)),
                    std::next(std::begin(lines), static_cast<long>(shift) + static_cast<long>(number_of_faces)),
                    std::begin(buffer_lines)
                );

                std::vector<shadow::Face> faces(buffer_lines.size());

                std::vector<std::size_t> indexes;
                std::istringstream sline;
                std::transform(
                    std::begin(buffer_lines),
                    std::end(buffer_lines),
                    std::begin(faces),
                    [&indexes, &sline, &faces](std::string line)
                    {
                        indexes.clear();

                        sline.clear();
                        sline.str(line);
                        std::size_t n(0);
                        sline >> n;
                        indexes.resize(n);

                        std::copy(
                            std::istream_iterator<std::size_t>(sline),
                            std::istream_iterator<std::size_t>(),
                            std::begin(indexes)
                        );

                        if (indexes.size() != n)
                            throw std::range_error("Error parsing facet! The number of points parsed do not match the number of points in the line.");
                        return shadow::Face(indexes);
                    }
                );

                return faces;
            }
        };
    }
}
