#pragma once

#include "../../shadow/mesh.h"

#include "../Line/line.h"

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
            * @param _ios reference to Adjacency stream
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
                /*Writing comments, header and sizes*/
                ios << "# Mesh: " << mesh.get_name() << std::endl
                    << "OFF" << std::endl
                    << mesh.points_size() << " " << mesh.faces_size() << " 0" << std::endl;

                /*Writing points*/
                std::for_each(
                    mesh.points_cbegin(),
                    mesh.points_cend(),
                    [this](std::pair<std::size_t, shadow::Point> const &_points)
                    {
                        ios << _points.second << std::endl;
                    }
                );

                /*Writing faces*/
                std::for_each(
                    mesh.faces_cbegin(),
                    mesh.faces_cend(),
                    [this](std::pair<std::size_t, shadow::Face> const &_facets)
                    {
                        ios << _facets.second << std::endl;
                    }
                );

                return *this;
            }
            /** 
             * Reads Mesh from off stream.
             * @param mesh the Mesh to write
             * @return the output stream
             */
            io::Off_stream & operator >>(shadow::Mesh & mesh)
            {
                std::stringstream error_message;
                
                /*Read Lines*/
                std::vector<std::string> lines;
                readlines(ios, std::back_inserter(lines));
                if (lines.empty())
                    throw std::out_of_range("The stream is empty!");

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
                    if (lines.empty())
                        throw std::out_of_range("The stream contains only the header; nothing to parse!");

                    /*Parsing sizes*/
                    std::vector<long> sizes(3);
                    std::stringstream _sizes(lines[1]);
                    std::copy(
                        std::istream_iterator<std::size_t>(_sizes),
                        std::istream_iterator<std::size_t>(),
                        std::begin(sizes)
                    );
                    if (sizes.size() != 3)
                        throw std::range_error("Error parsing the second line! There should be 3 integers.");
                    if (sizes[2] != 0 || sizes[0] < 0 || sizes[1] < 0)
                        throw std::range_error("Error parsing the second line! The first and second integers sould be positive and the third is always equal to 0.");
                    if (static_cast<long>(lines.size()) != (2 + sizes[0] + sizes[1]))
                        throw std::range_error("Error parsing the second line! The file should exactly contain the header, the sizes, the points and the faces: no more and no less.");

                    /*Parsing vertex points*/
                    std::vector<std::string> buffer_lines(static_cast<std::size_t>(sizes[0]));
                    std::copy(
                        std::next(std::begin(lines), 2),
                        std::next(std::begin(lines), 2 + sizes[0]),
                        std::begin(buffer_lines)
                    );

                    std::size_t idx(0);
                    std::map<std::size_t, shadow::Point> points;
                    std::stringstream sline;
                    for(auto const& line : buffer_lines)
                    {
                        sline.str(line);
                        std::vector<double> coordinates(3, 0);
                        std::copy(
                            std::istream_iterator<double>(sline),
                            std::istream_iterator<double>(),
                            std::begin(coordinates)
                        );
                        points.emplace(std::make_pair(idx++, shadow::Point(coordinates.at(0), coordinates.at(1), coordinates.at(2))));
                        sline.clear();
                    }

                    /*Parsing faces*/
                    idx = 0;
                    buffer_lines.clear();
                    sline.clear();

                    std::map<std::size_t, shadow::Face> faces;
                    buffer_lines = std::vector<std::string>(static_cast<std::size_t>(sizes[1]));
                    std::copy(
                        std::next(std::begin(lines), 2 + sizes[0]),
                        std::next(std::begin(lines), 2 + sizes[0] + sizes[1]),
                        std::begin(buffer_lines)
                    );

                    std::size_t n(0);
                    for(auto const& line : buffer_lines)
                    {
                        sline.str(line);
                        sline >> n;
                        std::vector<std::size_t> indexes(n, 0);
                        std::copy(
                            std::istream_iterator<std::size_t>(sline),
                            std::istream_iterator<std::size_t>(),
                            std::begin(indexes)
                        );
                        if (indexes.size() != n)
                            throw std::range_error("Error parsing facet! The number of points parsed do not match the number of points in the line.");
                        faces.emplace(std::make_pair(idx++, shadow::Face(indexes)));
                        sline.clear();
                    }

                    /*Mesh to return*/
                    mesh = shadow::Mesh(points, faces);
                }
                else
                    throw std::ios_base::failure("Not identified as OFF format! OFF files starts with a \'OFF\' hearder line.");
                return *this;
            }

        private:
            /** reference to a stream */
            std::iostream & ios;
        };
    }
}