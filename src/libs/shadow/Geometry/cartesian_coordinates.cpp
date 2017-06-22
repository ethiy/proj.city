#include "cartesian_coordinates.h"

#include "point.h"

#include <algorithm>
#include <numeric>

#include <limits>
#include <stdexcept>

#include <cmath>

namespace urban
{
    namespace shadow
    {
        Coordinates::Coordinates(void)
            : _coordinates{{0, 0, 0}}
        {}
        Coordinates::Coordinates(double x, double y, double z)
            : _coordinates{{x, y, z}}
        {}
        Coordinates::Coordinates(const double initializer[3])
            : _coordinates{{initializer[0], initializer[1], initializer[2]}}
        {}
        Coordinates::Coordinates(std::valarray<double> const& initializer)
            : _coordinates(initializer)
        {
            if(initializer.size() != 3)
                throw std::logic_error("The Coordinates size must be equal to 3!");
        }
        Coordinates::Coordinates(Coordinates const& other)
            : _coordinates(other._coordinates)
        {}
        Coordinates::Coordinates(Coordinates && other)
            : _coordinates(std::move(other._coordinates))
        {}
        Coordinates::~Coordinates(void)
        {}

        Coordinates::Coordinates(const Point & origin, const Point & target)
            : _coordinates{{target.x() - origin.x(), target.y() - origin.y(), target.z() - origin.z()}}
        {}
        Coordinates::Coordinates(Lib3dsPoint const& point)
            : _coordinates{{static_cast<double>(point.pos[0]), static_cast<double>(point.pos[1]), static_cast<double>(point.pos[2])}}
        {}
        Coordinates::Coordinates(Point_3 const& point)
            : _coordinates{{CGAL::to_double(point.x()), CGAL::to_double(point.y()), CGAL::to_double(point.z())}}
        {}

        double const& Coordinates::x(void) const noexcept
        {
            return _coordinates[0];
        }
        double const& Coordinates::y(void) const noexcept
        {
            return _coordinates[1];
        }
        double const& Coordinates::z(void) const noexcept
        {
            return _coordinates[2];
        }
        double & Coordinates::x(void) noexcept
        {
            return _coordinates[0];
        }
        double & Coordinates::y(void) noexcept
        {
            return _coordinates[1];
        }
        double & Coordinates::z(void) noexcept
        {
            return _coordinates[2];
        }

        Coordinates::iterator Coordinates::begin(void) noexcept
        {
            return std::begin(_coordinates);
        }
        Coordinates::const_iterator Coordinates::begin(void) const noexcept
        {
            return std::begin(_coordinates);
        }
        Coordinates::const_iterator Coordinates::cbegin(void) const noexcept
        {
            return std::begin(_coordinates);
        }
        Coordinates::iterator Coordinates::end(void) noexcept
        {
            return std::end(_coordinates);
        }
        Coordinates::const_iterator Coordinates::end(void) const noexcept
        {
            return std::end(_coordinates);
        }
        Coordinates::const_iterator Coordinates::cend(void) const noexcept
        {
            return std::end(_coordinates);
        }

        void Coordinates::swap(Coordinates & other)
        {
            using std::swap;
            swap(_coordinates, other._coordinates);
        }

        Coordinates & Coordinates::operator =(Coordinates const& other) noexcept
        {
            _coordinates = other._coordinates;
            return *this;
        }

        Coordinates & Coordinates::operator =(Coordinates && other) noexcept
        {
            _coordinates = std::move(other._coordinates);
            return *this;
        }

        Coordinates & Coordinates::operator +=(Coordinates const& other)
        {
            _coordinates += other._coordinates;
            return *this;
        }

        Coordinates & Coordinates::operator -=(Coordinates const& other)
        {
            _coordinates -= other._coordinates;
            return *this;
        }

        std::string Coordinates::toString(void) const noexcept
        {
            std::stringstream string;
            print(string);
            return string.str();
        }

        void Coordinates::print(std::ostream & os) const noexcept
        {
            os << _coordinates[0] << " " << _coordinates[1] << " " << _coordinates[2];
        }

        bool Coordinates::equals(Coordinates const& rhs) const noexcept
        {
            return (_coordinates == rhs._coordinates).min();
        }


        Coordinates Coordinates::adds(Coordinates const& rhs) const noexcept
        {
            return Coordinates(_coordinates + rhs._coordinates);
        }

        Coordinates Coordinates::minus(Coordinates const& rhs) const noexcept
        {
            return Coordinates(_coordinates - rhs._coordinates);
        }
    }

    void swap(shadow::Coordinates & lhs, shadow::Coordinates & rhs)
    {
        lhs.swap(rhs);
    }
}
