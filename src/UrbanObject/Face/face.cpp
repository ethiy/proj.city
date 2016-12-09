#include "face.h"

#include <boost/range/sub_range.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>

#include <algorithm>
#include <iterator>

namespace urban
{
    Face::Face(void){}
    Face::Face(const Face & other):points(other.points){}
    Face::Face(size_t first, size_t second, size_t third): points{{first, second, third}}{}
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

    std::vector<size_t> Face::get_indexes(void)
    {
        return points;
    }
    
    size_t Face::size(void)
    {
        return vertices_number;
    }

    void Face::invert_orientation(void)
    {
        std::vector<size_t> aux;
        std::reverse_copy(std::begin(points) + 1, std::end(points), aux);
        std::copy(std::begin(aux), std::end(aux), std::begin(points) +1 );
    }

    Lib3dsFace* Face::to_3ds()
    {
        Lib3dsFace* face = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), vertices_number-2));
        {
            boost::sub_range< std::vector<size_t> > twos(std::begin(points) + 1, std::end(points) - 1) , threes(std::begin(points) + 2, std::end(points));
            std::for_each(
                boost::make_zip_iterator(boost::make_tuple(std::begin(twos), std::begin(threes))),
                boost::make_zip_iterator(boost::make_tuple(std::end(twos), std::end(threes))),
                [&](boost::tuple<size_t, size_t> & two_three)
                {
                    face->points = reinterpret_cast<Lib3dsWord*>(calloc(sizeof(Lib3dWord), 3));
                    face->points = {points.at(0), boost::get<0>(two_three), boost::get<1>(two_three)};
                }
            );
        }
        return face;
    }

    std::ostream& operator<<(std::ostream & os, const Face & face)
    {
        os << face.vertices_number << " ";
        std::copy(std::begin(face.points), std::end(face.points), std::ostream_iterator<size_t>(os, " "));
        return os;
    }

    void swap(Face & lhs, Face &rhs)
    {
        lhs.swap(rhs);
    }
}
