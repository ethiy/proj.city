#pragma once

#include <array>
#include <ostream>
#include <initializer_list>

namespace urban
{
    namespace shadow
    {
        class Coordinates
        {
        public:
            Coordinates(void);
            Coordinates(double x, double y, double z);
            Coordinates(double coordinates[3]);
            Coordinates(std::initializer_list<double> initializer);
            Coordinates(Coordinates const& other);
            Coordinates(Coordinates && other);
            ~Coordinates(void);

            double x(void) const noexcept;
            double y(void) const noexcept;
            double z(void) const noexcept;

            void swap(Coordinates & other);

            Coordinates & operator=(Coordinates const& other) noexcept;
            Coordinates & operator=(Coordinates && other) noexcept;

            Coordinates & operator+=(Coordinates const& other);
            Coordinates & operator-=(Coordinates const& other);

        private:
            std::array<double, 3> coordinates;

            friend std::ostream & operator<<(std::ostream & os, Coordinates const& Coordinates);
            friend bool operator==(Coordinates const& lhs, Coordinates const& rhs);
            friend Coordinates & operator+(Coordinates & lhs, Coordinates const& rhs);
            friend Coordinates & operator-(Coordinates & lhs, Coordinates const& rhs);
        };
        bool operator!=(Coordinates const& lhs, Coordinates const& rhs);
    }

    void swap(shadow::Coordinates & lhs, shadow::Coordinates & rhs);
}
