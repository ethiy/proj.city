#pragma once

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Bbox
        {
        public:
            Bbox(void);
            Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
            Bbox(const Bbox & other);
            Bbox(Bbox && other);
            ~Bbox(void);

            double xmin(void) const noexcept;
            double xmax(void) const noexcept;
            double ymin(void) const noexcept;
            double ymax(void) const noexcept;
            double zmin(void) const noexcept;
            double zmax(void) const noexcept;

            void swap(Bbox & other);

            Bbox & operator=(const Bbox & other) noexcept;
            Bbox & operator=(Bbox && other) noexcept;

            Bbox & operator+=(const Bbox & other);

        private:
            std::array<double, 6> extreemes;
        };
        
        void swap(Bbox & lhs, Bbox & rhs);
        Bbox & operator+(Bbox & lhs, const Bbox & rhs);
        std::ostream & operator<<(std::ostream & os, Bbox & bbox);
    }
}
