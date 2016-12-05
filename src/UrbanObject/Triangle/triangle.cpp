#include "triangle.h"

#include <algorithm>
#include <iterator>

namespace urban
{
    Triangle::Triangle(void){}
    Triangle::Triangle(const Triangle & other):points(other.points){}
    Triangle::Triangle(size_t first, size_t second, size_t third):points{{first, second, third}}{}
    Triangle::~Triangle(void){}

    void Triangle::swap(Triangle & other)
    {
        using std::swap;
        swap(points, other.points);
    }

    Triangle & Triangle::operator=(Triangle other)
    {
        other.swap(*this);
        return *this;
    }

    size_t Triangle::operator[](size_t index)
    {
        return points[index];
    }

    void Triangle::invert_orientation(void)
    {
        points = {{points[0], points[2], points[1]}};
    }

    Lib3dsFace* Triangle::to_3ds()
    {
        Lib3dsFace* face = nullptr; //= malloc( 64 * sizeof char + 4 * sizeof(Lib3dsWord) + sizeof(Lib3dsDWord) + 3 * sizeof float );
        for(size_t i=0; i<3; ++i)
            face->points[i] = static_cast<Lib3dsWord>(points[i]);
        return face;
    }

    std::ostream& operator<<(std::ostream & os, const Triangle & triangle)
    {
        std::copy(std::begin(triangle.points), std::end(triangle.points), std::ostream_iterator<size_t>(os, " "));
        return os;
    }

    void swap(Triangle & lhs, Triangle &rhs)
    {
        lhs.swap(rhs);
    }
}
