#include "face.h"

#include "../Geometry/vector.h"

#include <algorithm>
#include <iterator>
#include <functional>

#include <stdexcept>

#include <cmath>

namespace urban
{
    namespace shadow
    {
        Face::Face(void)
        {}
        Face::Face(Face const& other)
            : points(other.points)
        {}
        Face::Face(Face && other)
            : points(std::move(other.points))
        {}
        Face::Face(std::initializer_list<std::size_t> initializer)
            : points(initializer)
        {
            if(initializer.size() < 3)
                throw std::logic_error("You must have at least three vertices to define a face!");
        }
        Face::Face(std::vector<std::size_t> const& indices)
            : points(indices)
        {
            if(indices.size() < 3)
                throw std::logic_error("You must have at least three vertices to define a face!");
        }
        Face::Face(std::size_t first, std::size_t second, std::size_t third, bool orientation)
            : points{{first, second, third}}
        {
            if(!orientation)
                invert_orientation();
        }
        Face::~Face(void) {}

        void Face::swap(Face & other)
        {
            using std::swap;
            swap(points, other.points);
        }
        Face & Face::operator =(Face const& other) noexcept
        {
            points = other.points;
            return *this;
        }
        Face & Face::operator =(Face && other) noexcept
        {
            points = std::move(other.points);
            return *this;
        }

        std::size_t & Face::operator [](std::size_t index)
        {
            return points[index];
        }
        std::size_t const& Face::operator [](std::size_t index) const
        {
            return points[index];
        }        

        std::size_t Face::degree(void) const noexcept
        {
            return points.size();
        }

        std::vector<std::size_t> & Face::data(void) noexcept
        {
            return points;
        }

        std::vector<std::size_t> const& Face::data(void) const noexcept
        {
            return points;
        }

        Face::iterator Face::begin(void) noexcept
        {
            return points.begin();
        }
        Face::iterator Face::end(void) noexcept
        {
            return points.end();
        }
        Face::const_iterator Face::begin(void) const noexcept
        {
            return points.begin();
        }
        Face::const_iterator Face::end(void) const noexcept
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
        void Face::rotate(void)
        {
            std::rotate(std::begin(points), std::next(std::begin(points)), std::end(points));
        }
        void Face::rotate(long n)
        {
            std::rotate(std::begin(points), std::next(std::begin(points), n), std::end(points));
        }

        Face::iterator Face::find(std::size_t index)
        {
            return std::find(std::begin(points), std::end(points), index);
        }
        bool Face::overide(std::size_t index, std::size_t new_index)
        {
            auto placeholder = find(index);
            bool success( placeholder != std::end(points));
            if(success)
                *placeholder = new_index;
            return success;
        }

        bool Face::is_convex(std::map<std::size_t, Point> const& coordinates) const
        {
            if(coordinates.size() < degree())
                throw std::out_of_range("The coordinates map must have at least the same size as the indexes registry");

            if(degree() < 3)
                throw std::out_of_range("You must have at least three vertices to define a face");

            bool convexity(true);

            /**
             * If the face is a triangle (i.e. 'degree == 3') it is convex (strictly if non-degenerate)
             */

            if(degree() > 3)
            {
                Vector normal_direction(Vector(coordinates.at(points.at(0)), coordinates.at(points.at(1))) ^ Vector(coordinates.at(points.at(1)), coordinates.at(points.at(2))));
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
                    Vector external_direction(Vector(A, B) ^ normal_direction);
                    convexity &= (external_direction * Vector(B, C) < 0) ;
                } while(convexity && ++circulator != std::end(points));
            }
            return convexity;
        }

        Lib3dsFace* Face::to_3ds(std::map<std::size_t, Point> const& coordinates) const
        {
            if(coordinates.size() < degree() )
                throw std::out_of_range("The coordinates map must have at least the same size as the indexes registry");
            Lib3dsFace* face = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), degree() - 2));
            if(is_convex(coordinates))
            {
                Lib3dsFace* current = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), 1));
                std::transform(
                    std::next(std::begin(points), 1),
                    std::prev(std::end(points), 1),
                    std::next(std::begin(points), 2),
                    face,
                    [this, &coordinates, current](std::size_t b, std::size_t c)
                    {
                        auto init = std::initializer_list<std::size_t>({points.at(0), b, c});
                        std::copy(std::begin(init), std::end(init), current->points);
                        Vector n = normal_to(coordinates.at(points.at(0)), coordinates.at(b), coordinates.at(c));
                        current->normal[0] = static_cast<float>(n.x());
                        current->normal[1] = static_cast<float>(n.y());
                        current->normal[2] = static_cast<float>(n.z());
                        return *current;
                    }
                );
                if(current)
                    std::free(current);
            }
            else
                throw std::logic_error("Cannot convert non convex faces to 3ds for now!");
            return face;
        }

        std::ostream& operator <<(std::ostream & os, Face const& face)
        {
            os << face.degree() << " ";
            auto it = std::begin(face.points);
            for(; it != std::end(face.points); ++it)
            {
                os << *it;
                if(it != std::prev(std::end(face.points)))
                    os << " ";
            }
            return os;
        }


        bool operator ==(Face const& lhs, Face const& rhs)
        {
            bool equal(lhs.degree() == rhs.degree());
            if(equal)
            {
                auto placeholder = std::find(std::begin(rhs.points),std::end(rhs.points), lhs.points.at(0));
                equal = (placeholder != std::end(rhs.points));
                if(equal)
                {
                    std::vector<std::size_t> rotated_points(lhs.degree());
                    std::rotate_copy(
                        std::begin(rhs.points),
                        placeholder,
                        std::end(rhs.points),
                        std::begin(rotated_points)
                    );
                    equal = std::inner_product(
                        std::begin(lhs.points),
                        std::end(lhs.points),
                        std::begin(rotated_points),
                        true,
                        std::logical_and<bool>(),
                        std::equal_to<std::size_t>()
                    );
                }
            }
            return equal;
        }
        bool operator !=(Face const& lhs, Face const& rhs)
        {
            return !(rhs == lhs);
        }

        void swap(Face & lhs, Face &rhs)
        {
            lhs.swap(rhs);
        }
    }
}
