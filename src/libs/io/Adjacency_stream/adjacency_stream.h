#pragma once

#include <ostream>

namespace urban
{
    class Adjacency_stream
    {
    public:
        Adjacency_stream(std::ostream & _os): os(_os) {}
        Adjacency_stream(std::ostream && _os): os(_os) {}
        Adjacency_stream(Adjacency_stream & other): os(other.os) {}
        Adjacency_stream(Adjacency_stream && other): os(other.os) {}

        template<typename T> Adjacency_stream & operator<<(const T & value)
        {
            os << value; 
            return *this;
        }

        Adjacency_stream & operator<<(std::ostream& (*func)(std::ostream&) )
        {
            func(os);
            return *this;
        }
    private:
        std::ostream & os;
    };
}
