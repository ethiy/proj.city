#include "face.h"

#include <algorithm>
#include <iterator>

namespace urban
{
    Face::Face(void){}
    Face::Face(const Face & other):points(other.points){}
    Face::Face(size_t first, size_t second, size_t third):vertices_number(3), points{{first, second, third}}{}
    Face::~Face(void){}

    void Face::swap(Face & other)
    {
        using std::swap;
        swap(vertices_number, other.vertices_number);
        swap(points, other.points);
    }

    Face & Face::operator=(Face other)
    {
        other.swap(*this);
        return *this;
    }

    size_t Face::operator[](size_t index)
    {
        return points[index];
    }

    void Face::invert_orientation(void)
    {
        points = {{points[0], points[2], points[1]}};
    }

    Lib3dsFace* Face::to_3ds()
    {
        Lib3dsFace* face = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace),1));
        std::copy(std::begin(points), std::end(points), face->points);
        return face;
    }

    std::ostream& operator<<(std::ostream & os, const Face & triangle)
    {
        std::copy(std::begin(triangle.points), std::end(triangle.points), std::ostream_iterator<size_t>(os, " "));
        return os;
    }

    void swap(Face & lhs, Face &rhs)
    {
        lhs.swap(rhs);
    }
}
