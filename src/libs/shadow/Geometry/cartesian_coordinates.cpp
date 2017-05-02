#include "cartesian_coordinates.h"

#include <algorithm>

#include <cmath>
#include <limits>
#include <stdexcept>

namespace urban
{
    namespace shadow
    {
        Coordinates::Coordinates(void): coordinates{{0, 0, 0}} {}
        Coordinates::Coordinates(double x, double y, double z): coordinates{{x, y, z}} {}
        Coordinates::Coordinates(Point const& origin, Point const& target): coordinates{{target.x() - origin.x(), target.y() - origin.y(), target.z() - origin.z()}} {}
        Coordinates::Coordinates(double _coordinates[3]): coordinates{{_coordinates[0], _coordinates[1], _coordinates[2]}} {}
        Coordinates(std::initializer_list<double> initializer): coordinates(initializer) {}
        Coordinates::Coordinates(Coordinates const& other): coordinates(other.coordinates) {}
        Coordinates::Coordinates(Coordinates && other): coordinates(std::move(other.coordinates)) {}
        Coordinates::~Coordinates(void){}

        double Coordinates::x(void) const noexcept
        {
            return coordinates.at(0);
        }
        double Coordinates::y(void) const noexcept
        {
            return coordinates.at(1);
        }
        double Coordinates::z(void) const noexcept
        {
            return coordinates.at(2);
        }

        void Coordinates::swap(Coordinates & other)
        {
            using std::swap;
            swap(coordinates, other.coordinates);
        }

        Coordinates & Coordinates::operator=(Coordinates const& other) noexcept
        {
            coordinates = other.coordinates;
            return *this;
        }

        Coordinates & Coordinates::operator=(Coordinates && other) noexcept
        {
            coordinates = std::move(other.coordinates);
            return *this;
        }

        Coordinates & Coordinates::operator+=(Coordinates const& other)
        {
            std::transform(
                std::begin(coordinates),
                std::end(coordinates),
                std::begin(other.coordinates),
                std::begin(coordinates),
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
                std::begin(coordinates),
                std::end(coordinates),
                std::begin(other.coordinates),
                std::begin(coordinates),
                [](const double lhs, const double rhs)
                {
                    return lhs - rhs;
                }
            );
            return *this;
        }

        Coordinates & Coordinates::operator^=(Coordinates const& other)
        {
            std::array<double, 3> coord(coordinates);
            coordinates[0] = coord.at(1) * other.z() - coord.at(2) * other.y();
            coordinates[1] = coord.at(2) * other.x() - coord.at(0) * other.z();
            coordinates[2] = coord.at(0) * other.y() - coord.at(1) * other.x();
            
            return *this;
        }

        std::ostream & operator<<(std::ostream & os, Coordinates const& Coordinates)
        {
            os << Coordinates.coordinates.at(0) << " " << Coordinates.coordinates.at(1) << " " << Coordinates.coordinates.at(2);
            return os;
        }

        bool operator==(Coordinates const& lhs, Coordinates const& rhs)
        {
            return  std::abs(lhs.x() - rhs.x()) < std::numeric_limits<double>::epsilon()
                    &&
                    std::abs(lhs.y() - rhs.y()) < std::numeric_limits<double>::epsilon()
                    &&
                    std::abs(lhs.z() - rhs.z()) < std::numeric_limits<double>::epsilon();
        }


        Coordinates & operator+(Coordinates & lhs, Coordinates const& rhs)
        {
            return lhs += rhs;
        }

        Coordinates & operator-(Coordinates & lhs, Coordinates const& rhs)
        {
            return lhs -= rhs;
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
