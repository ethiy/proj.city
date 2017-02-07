#include "face.h"

#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Projection_traits_xz_3.h>
#include <CGAL/Projection_traits_yz_3.h>

#include <boost/range/combine.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/foreach.hpp>

#include <algorithm>
#include <iterator>

#include <stdexcept>

namespace urban
{
    Face::Face(void): vertices_number(0){}
    Face::Face(const Face & other): vertices_number(other.vertices_number), points(other.points){}
    Face::Face(size_t first, size_t second, size_t third): vertices_number(3), points{{first, second, third}}{}
    Face::Face(size_t _vertices_number, std::vector<size_t> _points):vertices_number(_vertices_number), points(_points)
    {
        if(_vertices_number<3)
            throw std::out_of_range("You must have at least three vertices to define a face!");
    }
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

    
    size_t Face::size(void) const noexcept
    {
        return vertices_number;
    }

    Face::iterator Face::begin(void) noexcept
    {
        return points.begin();
    }
    Face::iterator Face::end(void) noexcept
    {
        return points.end();
    }
    Face::const_iterator Face::cbegin(void) const noexcept
    {
        return points.cbegin();
    }
    Face::const_iterator Face::cend(void) const noexcept
    {
        return points.cend();
    }

    void Face::invert_orientation(void)
    {
        std::vector<size_t> aux(vertices_number-1);
        std::reverse_copy( std::next(std::begin(points), 1), std::end(points), std::begin(aux));
        std::copy(std::begin(aux), std::end(aux), std::next(std::begin(points), 1) );
    }

    bool Face::is_convex(std::map<size_t, Point> & coordinates) const
    {
        if(vertices_number < 3)
            throw std::out_of_range("You must have at least three vertices to define a face!");

        bool convexity(true);

        /*
         * If the face is a triangle (i.e. 'vertices_number == 3') it is convex (strictly if non-degenerate)
         */
        if(vertices_number > 3)
        {
            Vector normal(CGAL::unit_normal(coordinates.at(points.at(1)), coordinates.at(points.at(2)), coordinates.at(points.at(0))));
            using points_iterator = std::vector<size_t>::iterator;
            points_iterator circulator(std::begin(points));
            do
            {
                points_iterator next_1, next_2;
                if(circulator == std::prev(std::end(points),2))
                {
                    next_1 = std::next(circulator, 1);
                    next_2 = std::begin(points);
                }
                else
                {
                    if(circulator == std::prev(std::end(points),1))
                    {
                        next_1 = std::begin(points);
                        next_2 = std::next(next_1, 1);
                    }
                    else
                    {
                        next_1 = std::next(circulator, 1);
                        next_2 = std::next(circulator, 2);
                    }
                }
                Point A(coordinates[*circulator]), B(coordinates[*next_1]), C(coordinates[*next_2]);
                Vector internal_direction(CGAL::normal(B, B + normal, A));
                convexity &= (internal_normal * Vector(B, C) > 0) ;
            } while(convexity && ++circulator != std::end(points));
        }
        return convexity;
    }

    Lib3dsFace* Face::to_3ds(std::map<size_t, Point> & coordinates)
    {
        Lib3dsFace* face = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), vertices_number-2));
        if(is_convex(coordinates))
        {
            std::transform(
                std::next(std::begin(points), 1),
                std::prev(std::end(points), 1),
                std::next(std::begin(points), 2),
                face,
                [this](size_t b, size_t c)
                {
                    Lib3dsFace current;
                    auto init = std::initializer_list<size_t>({points.at(0), b, c});
                    std::copy(std::begin(init), std::end(init), current.points);
                    return current;
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
