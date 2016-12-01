#include "triangle.h"

namespace urban
{
    Triangle::Triangle(void){}
    Triangle::Triangle(const Triangle & other):points(other.points){}
    Triangle::Triangle(size_t first, size_t second, size_t third):points(std::make_tuple(first, second, third)){}
    Triangle::~Triangle(void){}

    size_t operator[](size_t index)
    {
        return points[index];
    }

    void Triangle::invert_orientation(void)
    {
        points = std::make_tuple(points[0], points[2], points[1]);
    }
}