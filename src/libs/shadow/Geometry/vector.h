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
            Vector(void);
            Vector(double x, double y, double z);
            Vector(const double initializer[3]);
            Vector(std::valarray<double> const& initializer);
            Vector(Vector const& other);
            Vector(Vector && other);
            Vector(const Point & origin, const Point & target);
            ~Vector(void);

            void swap(Vector & other);
            Vector & operator =(Vector const& other);
            Vector & operator =(Vector && other);

            Vector & operator +=(Vector const& other);
            Vector & operator *=(double const& scalar);
            Vector & operator /=(double const& scalar);
            Vector & operator ^=(Vector const& other);
        protected:
            friend bool operator ==(Vector const& lhs, Vector const& rhs);

            friend Vector operator +(Vector const& lhs, Vector const& rhs);
            friend Vector operator -(Vector const& lhs, Vector const& rhs);

            friend double operator *(Vector const& lhs, Vector const& rhs);
            friend Vector operator *(double const& scalar, Vector const& rhs);
            friend Vector operator /(Vector const& lhs, double const& scalar);

            friend Vector operator ^(Vector const& lhs, Vector const& rhs);

            friend std::ostream & operator <<(std::ostream & os, Vector const& vector);
        };
        void swap(Vector & lhs, Vector & rhs);

        bool operator !=(Vector const& lhs, Vector const& rhs);

        double norm_L2(Vector const& vector);
        double determinant(const Vector & first, const Vector & second, const Vector & third);
    }
}
