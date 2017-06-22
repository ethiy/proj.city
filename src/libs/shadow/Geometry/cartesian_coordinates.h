#pragma once

#include "../../geometry_definitions.h"

#include <lib3ds/mesh.h>

#include <valarray>
#include <vector>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Point;
        
        class Coordinates
        {
        public:
            Coordinates(void);
            Coordinates(double x, double y, double z);
            Coordinates(const double initializer[3]);
            Coordinates(std::valarray<double> const& initializer);
            Coordinates(Coordinates const& other);
            Coordinates(Coordinates && other);
            Coordinates(const Point & origin, const Point & target);
            Coordinates(Lib3dsPoint const& point);
            Coordinates(Point_3 const& point);
            virtual ~Coordinates(void);

            std::array<double, 3> const& data(void) const noexcept;
            std::array<double, 3> & data(void) noexcept;
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
            Coordinates & operator =(Coordinates const& other) noexcept;
            Coordinates & operator =(Coordinates && other) noexcept;

            Coordinates & operator +=(Coordinates const& other);
            Coordinates & operator -=(Coordinates const& other);

            std::string toString(void) const noexcept;
        protected:
            std::valarray<double> _coordinates;

            virtual void print(std::ostream & os) const noexcept;
            virtual bool equals(Coordinates const& other) const noexcept;
            virtual Coordinates adds(Coordinates const& other) const noexcept;
            virtual Coordinates minus(Coordinates const& other) const noexcept;
        };
        void swap(Coordinates & lhs, Coordinates & rhs);
    }
}
