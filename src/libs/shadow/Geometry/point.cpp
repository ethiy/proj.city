#include "point.h"

#include "vector.h"

#include <CGAL/number_utils.h>

namespace urban
{
    namespace shadow
    {
        Point::Point(void)
        {}
        Point::Point(double x, double y, double z)
            : Coordinates(x, y, z)
        {}
        Point::Point(const double initializer[3])
            : Coordinates(initializer)
        {}
        Point::Point(std::valarray<double> const& initializer)
            : Coordinates(initializer)
        {}
        Point::Point(Point const& other)
            : Coordinates(other)
        {}
        Point::Point(Point && other)
            : Coordinates(other)
        {}
        Point::Point(Lib3dsPoint const& point)
            : Coordinates(point)
        {}
        Point::Point(Point_3 const& point)
            : Coordinates(point)
        {}
        Point::~Point(void)
        {}


        Bbox Point::bbox(void) const
        {
            return Bbox(_coordinates);
        }

        void Point::swap(Point & other)
        {
            Coordinates::swap(other);
        }

        Point & Point::operator =(Point const& other) noexcept
        {
            Coordinates::operator =(other);
            return *this;
        }
        Point & Point::operator =(Point && other) noexcept
        {
            Coordinates::operator =(other);
            return *this;
        }

        bool operator ==(Point const& lhs, Point const& rhs)
        {
            return lhs.equals(rhs);
        }
        Vector operator -(Point const& lhs, Point const& rhs)
        {
            return Vector(lhs, rhs);
        }

        std::ostream & operator <<(std::ostream & os, Point const& rhs)
        {
            rhs.print(os);
            return os;
        }

        void swap(Point & lhs, Point & rhs)
        {
            lhs.swap(rhs);
        }

        bool operator !=(Point const& lhs, Point const& rhs)
        {
            return !(lhs == rhs);
        }

        Vector normal_to(Point const& first, Point const& second, Point const& third)
        {
            Vector v(first, second);
            v ^= Vector(second, third);
            return v / norm_L2(v);
        }
    }    
}
