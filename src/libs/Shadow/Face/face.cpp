#include "face.h"

#include <CGAL/squared_distance_3.h>

#include <algorithm>
#include <iterator>

#include <stdexcept>

#include <cmath>

namespace urban
{
    namespace shadow
    {
        Face::Face(void): vertices_number(0){}
        Face::Face(const Face & other): vertices_number(other.vertices_number), points(other.points){}
        Face::Face(Face && other): vertices_number(std::move(other.vertices_number)), points(std::move(other.points)){}
        Face::Face(size_t first, size_t second, size_t third): vertices_number(3), points{{first, second, third}}{}
        Face::Face(size_t _vertices_number, const std::vector<size_t> & _points):vertices_number(_vertices_number), points(_points)
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

        Face & Face::operator=(const Face & other) noexcept
        {
            vertices_number = other.vertices_number;
            points.resize(vertices_number);
            std::copy(std::begin(other.points), std::begin(other.points), std::begin(points));
            return *this;
        }

        Face & Face::operator=(Face && other) noexcept
        {
            vertices_number = std::move(other.vertices_number);
            points = std::move(other.points);
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
            std::reverse(std::next(std::begin(points)), std::end(points));
        }

        Face::iterator Face::find(size_t index)
        {
            return std::find(std::begin(points), std::end(points), index);
        }

        bool Face::overide(size_t index, size_t new_index)
        {
            auto placeholder = find(index);
            bool success( placeholder != std::end(points));
            if(success)
                *placeholder = new_index;
            return success;
        }

        bool Face::is_convex(const std::map<size_t, Point> & coordinates) const
        {
            if(coordinates.size() < vertices_number)
                throw std::out_of_range("The coordinates map must have at least the same size as the indexes registry");

            if(vertices_number < 3)
                throw std::out_of_range("You must have at least three vertices to define a face");

            bool convexity(true);

            /*
            * If the face is a triangle (i.e. 'vertices_number == 3') it is convex (strictly if non-degenerate)
            */

            if(vertices_number > 3)
            {
                Vector normal(CGAL::normal(coordinates.at(points.at(1)), coordinates.at(points.at(2)), coordinates.at(points.at(0))));
                auto circulator = std::begin(points);
                do
                {
                    decltype(circulator) next_1, next_2;
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
                    Point A(coordinates.at(*circulator)), B(coordinates.at(*next_1)), C(coordinates.at(*next_2));
                    Vector external_direction(CGAL::normal(B, B + normal, A));
                    convexity &= (external_direction * Vector(B, C) < 0) ;
                } while(convexity && ++circulator != std::end(points));
            }
            return convexity;
        }

        Lib3dsFace* Face::to_3ds(const std::map<size_t, Point> & coordinates)
        {
            if(coordinates.size() < vertices_number)
                throw std::out_of_range("The coordinates map must have at least the same size as the indexes registry");
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
            else
                throw std::logic_error("Cannot convert non convex faces to 3ds for now");
            return face;
        }

        std::ostream& operator<<(std::ostream & os, const Face & face)
        {
            os << face.vertices_number << " ";
            std::copy(std::begin(face.points), std::end(face.points), std::ostream_iterator<size_t>(os, " "));
            return os;
        }
    }
    void swap(shadow::Face & lhs, shadow::Face &rhs)
    {
        lhs.swap(rhs);
    }
}
