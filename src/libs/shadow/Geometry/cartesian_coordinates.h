#pragma once

#include "../../geometry_definitions.h"

#include <lib3ds/mesh.h>

#include <array>
#include <vector>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Point;
        class Vector;

        class Coordinates
        {
        public:
            Coordinates(void);
            Coordinates(double x, double y, double z);
            Coordinates(double initializer[3]);
            Coordinates(std::array<double, 3> const& initializer);
            Coordinates(Coordinates const& other);
            Coordinates(Coordinates && other);
            ~Coordinates(void);

            double const& x(void) const noexcept;
            double const& y(void) const noexcept;
            double const& z(void) const noexcept;
            double & x(void) noexcept;
            double & y(void) noexcept;
            double & z(void) noexcept;

            using iterator = std::array<double, 3>::iterator;
            using const_iterator = std::array<double, 3>::const_iterator;

            iterator begin(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            iterator end(void) noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cend(void) const noexcept;

            void swap(Coordinates & other);

            Coordinates & operator=(Coordinates const& other) noexcept;
            Coordinates & operator=(Coordinates && other) noexcept;

            Coordinates & operator+=(Coordinates const& other);
            Coordinates & operator-=(Coordinates const& other);
        protected:
            Coordinates(const Point & origin, const Point & target);
            Coordinates(Lib3dsPoint const& point);
            Coordinates(Point_3 const& point);

            std::array<double, 3> _coordinates;
        private:
            
            friend std::ostream & operator<<(std::ostream & os, Coordinates const& Coordinates);
            friend bool operator==(Coordinates const& lhs, Coordinates const& rhs);
            friend Coordinates operator+(Coordinates const& lhs, Coordinates const& rhs);
            friend Coordinates operator-(Coordinates const& lhs, Coordinates const& rhs);
        };
        bool operator!=(Coordinates const& lhs, Coordinates const& rhs);
    }

    void swap(shadow::Coordinates & lhs, shadow::Coordinates & rhs);
}
