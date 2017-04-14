#pragma once

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace urban
{
    namespace io
    {
        /**
         * @ingroup io
         * @brief Defines a Line in order to easily parse text streams.
         */
        class Line
        {
        public:
            /**
             * Default Constructor
             * @see Line(const Line & other);
             * @see Line(Line && other);
             * @see Line(const std::string & _line);
             * @see ~Line(void);
             */
            Line(void) {}
            /**
             * Copy Constructor
             * @param other Line to copy
             * @see Line(void);
             * @see Line(Line && other);
             * @see Line(const std::string & _line);
             * @see ~Line(void);
             */
            Line(const Line & other): line(other.line){}
            /**
             * Move Constructor
             * @param other Line to move
             * @see Line(void);
             * @see Line(const Line & other);
             * @see Line(const std::string & _line);
             * @see ~Line(void);
             */
            Line(Line && other): line(std::move(other.line)){}
            /**
             * Constructor from STL string
             * @param _line Line content
             * @see Line(void);
             * @see Line(const Line & other);
             * @see Line(Line && other);
             * @see ~Line(void);
             */
            Line(const std::string & _line): line(_line){}
            /**
             * Default Destructor
             * @see Line(void);
             * @see Line(const Line & other);
             * @see Line(Line && other);
             * @see Line(const std::string & _line);
             */
            ~Line(void) {}

            /**
             * STL string cast
             */
            operator std::string() const
            {
                return line;
            }

        private:
            /** line as STL string */
            std::string line;
            /**
             * Input streamer
             * @param is input stream
             * @param l line to input
             * return reference to the output stream
             * @see std::ostream & operator<<(std::ostream & os, const Line & line);
             */
            friend std::istream & operator>>(std::istream & is, Line & l)
            {
                std::getline(is, l.line);
                return is;
            }
            /**
             * Output streamer streamer
             * @param os output stream
             * @param l line to output
             * return reference to the output stream
             * @see std::istream & operator>>(std::istream & is, Line & l);
             */
            friend std::ostream & operator<<(std::ostream & os, const Line & l)
            {
                os << l.line << std::endl;
                return os;
            }
        };

        template<class output_iterator>
        void readlines(std::istream & is, output_iterator destination_iterator)
        {
            std::copy(std::istream_iterator<Line>(is), std::istream_iterator<Line>(), destination_iterator);
        }
    }
}
