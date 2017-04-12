#pragma once

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace urban
{
    namespace io
    {
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
            Line(void);
            /**
             * Copy Constructor
             * @see Line(void);
             * @see Line(Line && other);
             * @see Line(const std::string & _line);
             * @see ~Line(void);
             */
            Line(const Line & other);
            /**
             * Move Constructor
             * @see Line(void);
             * @see Line(const Line & other);
             * @see Line(const std::string & _line);
             * @see ~Line(void);
             */
            Line(Line && other);
            /**
             * Constructor from STL string
             * @see Line(void);
             * @see Line(const Line & other);
             * @see Line(Line && other);
             * @see ~Line(void);
             */
            Line(const std::string & _line);
            /**
             * Default Destructor
             * @see Line(void);
             * @see Line(const Line & other);
             * @see Line(Line && other);
             * @see Line(const std::string & _line);
             */
            ~Line(void);

            /**
             * STL string cast
             */
            operator std::string() const;

        private:
            /** line as STL string */
            std::string line;
            /**
             * Input streamer
             * @param is input stream
             * @param line line to input
             * @see std::ostream & operator<<(std::ostream & os, const Line & line);
             */
            friend std::istream & operator>>(std::istream & is, Line & line);
            /**
             * Output streamer streamer
             * @param os output stream
             * @param line line to output
             * @see std::istream & operator>>(std::istream & is, Line & line);
             */
            friend std::ostream & operator<<(std::ostream & os, const Line & line);
        };

        template<class output_iterator>
        void read_lines(std::istream & is, output_iterator destination_iterator)
        {
            std::copy(std::istream_iterator<Line>(is), std::istream_iterator<Line>(), destination_iterator);
        }
    }
}
