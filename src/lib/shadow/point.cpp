#include <shadow/point.h>

#include <shadow/vector.h>
#include <shadow/bbox.h>

#include <CGAL/number_utils.h>

namespace city
{
    namespace shadow
    {
        Point::Point(void)
            : coordinates{{0, 0, 0}}
        {}
        Point::Point(double x, double y, double z)
            : coordinates{{x, y, z}}
        {}
        Point::Point(double _coordinates[3])
            : coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}}
        {}
        Point::Point(std::valarray<double> const& initializer)
            : coordinates(initializer)
        {
            if(initializer.size() != 3)
                throw std::logic_error("Cannot create a 3D Point from this entry! Check the size of the initializer.");
        }
        Point::Point(Point_3 const& point)
            : coordinates{{CGAL::to_double(point.x()), CGAL::to_double(point.y()), CGAL::to_double(point.z())}}
        {}
        Point::Point(Lib3dsPoint const& point)
            : coordinates{{static_cast<double>(point.pos[0]), static_cast<double>(point.pos[1]), static_cast<double>(point.pos[2])}}
        {}
        Point::Point(Point const& other)
            : coordinates(other.coordinates)
        {}
        Point::Point(Point && other)
            : coordinates(std::move(other.coordinates))
        {}
        Point::~Point(void)
        {}

        std::valarray<double> const& Point::data(void) const noexcept
        {
            return coordinates;
        }
        std::valarray<double> & Point::data(void) noexcept
        {
            return coordinates;
        }

        double const& Point::x(void) const noexcept
        {
            return coordinates[0];
        }
        double const& Point::y(void) const noexcept
        {
            return coordinates[1];
        }
        double const& Point::z(void) const noexcept
        {
            return coordinates[2];
        }
        double & Point::x(void) noexcept
        {
            return coordinates[0];
        }
        double & Point::y(void) noexcept
        {
            return coordinates[1];
        }
        double & Point::z(void) noexcept
        {
            return coordinates[2];
        }

        void Point::swap(Point & other)
        {
            using std::swap;
            swap(coordinates, other.coordinates);
        }
        Point & Point::operator =(Point const& other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }
        Point & Point::operator =(Point && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Point & Point::operator +=(Vector const& translation)
        {
            coordinates += translation.data();
            return *this;
        }

        Bbox Point::bbox(void) const
        {
            return Bbox(*this);
        }

        Point_3 Point::to_cgal(void) const
        {
            return Point_3(x(), y(), z());
        }

        Point operator +(Point const& lhs, Vector const& rhs)
        {
            return Point(lhs.coordinates + rhs.data());
        }
        bool operator ==(Point const& lhs, Point const& rhs)
        {
            return (lhs.coordinates == rhs.coordinates).min();
        }

        std::ostream & operator <<(std::ostream & os, Point const& point)
        {
            os << point.coordinates[0] << " " << point.coordinates[1] << " " << point.coordinates[2];
            return os;
        }
        
        Vector operator -(Point const& lhs, Point const& rhs)
        {
            return Vector(rhs, lhs);
        }        
        bool operator !=(Point const& lhs, Point const& rhs)
        {
            return !(rhs == lhs);
        }

        void swap(Point & lhs, Point & rhs)
        {
            lhs.swap(rhs);
        }
        Vector normal_to(Point const& first, Point const& second, Point const& third)
        {
            Vector v(first, second);
            v ^= Vector(second, third);
            return v / norm_L2(v);
        }
    }
}
