#pragma once

#include <string>
#include <iostream>

namespace urban
{
    namespace io
    {
        class Line
        {
        public:
            Line(void);
            Line(const Line &);
            Line(std::string);
            ~Line(void);

            operator std::string() const;

        private:
            std::string line;
            friend std::istream & operator>>(std::istream &, Line &);
            friend std::ostream & operator<<(std::ostream &, const Line &);
        };

        template<class output_iterator>
        void read_lines(std::istream& is, output_iterator destination_iterator);
    }
}
