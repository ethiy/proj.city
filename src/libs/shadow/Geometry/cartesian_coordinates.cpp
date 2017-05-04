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
        Coordinates::Coordinates(double initializer[3])
            : _coordinates{{initializer[0], initializer[1], initializer[2]}}
        {}
        Coordinates::Coordinates(std::array<double, 3> const& initializer)
            : _coordinates(initializer)
        {}
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
            return _coordinates.at(0);
        }
        double const& Coordinates::y(void) const noexcept
        {
            return _coordinates.at(1);
        }
        double const& Coordinates::z(void) const noexcept
        {
            return _coordinates.at(2);
        }
        double & Coordinates::x(void) noexcept
        {
            return _coordinates.at(0);
        }
        double & Coordinates::y(void) noexcept
        {
            return _coordinates.at(1);
        }
        double & Coordinates::z(void) noexcept
        {
            return _coordinates.at(2);
        }

        Coordinates::iterator Coordinates::begin(void) noexcept
        {
            return _coordinates.begin();
        }
        Coordinates::const_iterator Coordinates::begin(void) const noexcept
        {
            return _coordinates.begin();
        }
        Coordinates::const_iterator Coordinates::cbegin(void) const noexcept
        {
            return _coordinates.cbegin();
        }
        Coordinates::iterator Coordinates::end(void) noexcept
        {
            return _coordinates.end();
        }
        Coordinates::const_iterator Coordinates::end(void) const noexcept
        {
            return _coordinates.end();
        }
        Coordinates::const_iterator Coordinates::cend(void) const noexcept
        {
            return _coordinates.cend();
        }

        void Coordinates::swap(Coordinates & other)
        {
            using std::swap;
            swap(_coordinates, other._coordinates);
        }

        Coordinates & Coordinates::operator=(Coordinates const& other) noexcept
        {
            _coordinates = other._coordinates;
            return *this;
        }

        Coordinates & Coordinates::operator=(Coordinates && other) noexcept
        {
            _coordinates = std::move(other._coordinates);
            return *this;
        }

        Coordinates & Coordinates::operator+=(Coordinates const& other)
        {
            std::transform(
                std::begin(_coordinates),
                std::end(_coordinates),
                std::begin(other._coordinates),
                std::begin(_coordinates),
                [](const double lhs, const double rhs)
                {
                    return lhs + rhs;
                }
            );
            return *this;
        }

        Coordinates & Coordinates::operator-=(Coordinates const& other)
        {
            std::transform(
                std::begin(_coordinates),
                std::end(_coordinates),
                std::begin(other._coordinates),
                std::begin(_coordinates),
                [](const double lhs, const double rhs)
                {
                    return lhs - rhs;
                }
            );
            return *this;
        }

        std::ostream & operator<<(std::ostream & os, Coordinates const& Coordinates)
        {
            os << Coordinates._coordinates.at(0) << " " << Coordinates._coordinates.at(1) << " " << Coordinates._coordinates.at(2);
            return os;
        }

        bool operator==(Coordinates const& lhs, Coordinates const& rhs)
        {
            return std::inner_product(
                std::begin(lhs),
                std::end(lhs),
                std::begin(rhs),
                true,
                std::logical_and<bool>(),
                [](double const& r, double const& l)
                {
                    return std::abs(r - l) < std::numeric_limits<double>::epsilon();
                }
            );
        }


        Coordinates operator+(Coordinates const& lhs, Coordinates const& rhs)
        {
            Coordinates result;
            std::transform(
                std::begin(lhs._coordinates),
                std::end(lhs._coordinates),
                std::begin(rhs._coordinates),
                std::begin(result),
                [](double const& _lhs, double const& _rhs)
                {
                    return _lhs + _rhs;
                }
            );
            return result;
        }

        Coordinates operator-(Coordinates const& lhs, Coordinates const& rhs)
        {
            Coordinates result;
            std::transform(
                std::begin(lhs._coordinates),
                std::end(lhs._coordinates),
                std::begin(rhs._coordinates),
                std::begin(result),
                [](double const& _lhs, double const& _rhs)
                {
                    return _lhs - _rhs;
                }
            );
            return result;
        }

        bool operator!=(Coordinates const& lhs, Coordinates const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(shadow::Coordinates & lhs, shadow::Coordinates & rhs)
    {
        lhs.swap(rhs);
    }
}
