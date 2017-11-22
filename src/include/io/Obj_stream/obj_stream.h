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
    }
}
