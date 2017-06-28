#include "bbox.h"

#include <algorithm>

#include <limits>
#include <iterator>
#include <algorithm>

namespace urban
{
    namespace shadow
    {
        Bbox::Bbox(void)
            : extremes{{
                std::numeric_limits<double>::infinity(),
                - std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity(),
                - std::numeric_limits<double>::infinity(),
                std::numeric_limits<double>::infinity(),
                - std::numeric_limits<double>::infinity()
            }}
        {}
        Bbox::Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
            : extremes{{
                xmin,
                xmax,
                ymin,
                ymax,
                zmin,
                zmax
            }}
        {}
        Bbox::Bbox(std::valarray<double> const& coordinates)
            : extremes{{
                coordinates[0],
                coordinates[0],
                coordinates[1],
                coordinates[1],
                coordinates[2],
                coordinates[2],
            }}
        {}
        Bbox::Bbox(Bbox const& other)
            : extremes(other.extremes) {}
        Bbox::Bbox(Bbox && other)
            : extremes(std::move(other.extremes)) {}
        Bbox::~Bbox(void) {}

        double const& Bbox::xmin(void) const noexcept
        {
            return extremes[0];
        }
        double const& Bbox::xmax(void) const noexcept
        {
            return extremes[1];
        }
        double const& Bbox::ymin(void) const noexcept
        {
            return extremes[2];
        }
        double const& Bbox::ymax(void) const noexcept
        {
            return extremes[3];
        }
        double const& Bbox::zmin(void) const noexcept
        {
            return extremes[4];
        }
        double const& Bbox::zmax(void) const noexcept
        {
            return extremes[5];
        }

        double & Bbox::xmin(void) noexcept
        {
            return extremes[0];
        }
        double & Bbox::xmax(void) noexcept
        {
            return extremes[1];
        }
        double & Bbox::ymin(void) noexcept
        {
            return extremes[2];
        }
        double & Bbox::ymax(void) noexcept
        {
            return extremes[3];
        }
        double & Bbox::zmin(void) noexcept
        {
            return extremes[4];
        }
        double & Bbox::zmax(void) noexcept
        {
            return extremes[5];
        }

        void Bbox::swap(Bbox & other)
        {
            using std::swap;
            swap(extremes, other.extremes);
        }

        Bbox & Bbox::operator =(Bbox const& other) noexcept
        {
            extremes = other.extremes;
            return *this;
        }

        Bbox & Bbox::operator =(Bbox && other) noexcept
        {
            extremes = std::move(other.extremes);
            return *this;
        }

        Bbox & Bbox::operator +=(Bbox const& other)
        {
            extremes[extremes[std::slice(0, 3, 2)] <= other.extremes[std::slice(0, 3, 2)]] = other.extremes[extremes[std::slice(0, 3, 2)] <= other.extremes[std::slice(0, 3, 2)]];
            extremes[extremes[std::slice(1, 3, 2)] <= other.extremes[std::slice(1, 3, 2)]] = other.extremes[extremes[std::slice(1, 3, 2)] >= other.extremes[std::slice(1, 3, 2)]];
            
            return *this;
        }

        Bbox operator +(Bbox const& lhs, Bbox const& rhs)
        {
            Bbox copy(lhs);
            copy += rhs;
            return copy += rhs;
        }

        CGAL::Bbox_3 Bbox::to_cgal(void) const noexcept
        {
            return CGAL::Bbox_3(extremes[0], extremes[2], extremes[4], extremes[1], extremes[3], extremes[5]);
        }

        std::ostream & operator <<(std::ostream & os, Bbox const& bbox)
        {
            os << bbox.extremes[0] << " " << bbox.extremes[1] << " " << bbox.extremes[2] << " " << bbox.extremes[3] << " " << bbox.extremes[4] << " " << bbox.extremes[5];
            return os;
        }
    }

    void swap(shadow::Bbox & lhs, shadow::Bbox & rhs)
    {
        lhs.swap(rhs);
    }

}
