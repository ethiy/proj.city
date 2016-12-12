#include "line.h"

namespace urban
{
    namespace io
    {
        Line::Line(void){}
        Line::Line(const Line & other): line(other.line){}
        Line::Line(std::string s):line(s){}
        Line::~Line(void){}

        Line::operator std::string() const
        {
            return line;
        }

        std::istream & operator>>(std::istream & is, Line & l)
        {
            std::getline(is, l.line);
            return is;
        }

        std::ostream & operator<<(std::ostream & os, const Line & l)
        {
            os << l.line << std::endl;
            return os;
        }
    }
}