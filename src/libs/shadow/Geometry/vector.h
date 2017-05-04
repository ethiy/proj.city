#pragma once

#include "cartesian_coordinates.h"

namespace urban
{
    namespace shadow
    {
        class Point;

        class Vector : public Coordinates
        {
        public:
            using Coordinates::Coordinates;
            Vector(const Point & origin, const Point & target);

            Vector & operator+=(Vector const& other);
            Vector & operator*=(double const& scalar);
            Vector & operator/=(double const& scalar);
            Vector & operator^=(Vector const& other);
        };

        double operator*(Vector const& lhs, Vector const& rhs);
        Vector operator*(double const& scalar, Vector const& rhs);
        Vector operator/(Vector const& lhs, double const& scalar);
        Vector operator^(Vector const& lhs, Vector const& rhs);
    }

    double norm_L2(shadow::Vector const& vector);
    double determinant(const shadow::Vector & first, const shadow::Vector & second, const shadow::Vector & third);
}
