#include "triangle.h"

#include <algorithm>
#include <iterator>

namespace urban
{
    Triangle::Triangle(void){}
    Triangle::Triangle(const Triangle & other):points(other.points){}
    Triangle::Triangle(size_t first, size_t second, size_t third):points{first, second, third}{}
    Triangle::~Triangle(void){}

    size_t Triangle::operator[](size_t index)
    {
        return points[index];
    }

    void Triangle::invert_orientation(void)
    {
        points = {points[0], points[2], points[1]};
    }

    std::ostream& operator<<(std::ostream & os, const Triangle & triangle)
    {
        std::copy(std::begin(triangle.points), std::end(triangle.points), std::ostream_iterator<size_t>(os, " "));
        return os;
    }
}