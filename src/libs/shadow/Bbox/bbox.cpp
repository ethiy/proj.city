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
            : mins{{
                    std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity()
              }},
              maxes{{
                    - std::numeric_limits<double>::infinity(),
                    - std::numeric_limits<double>::infinity(),
                    - std::numeric_limits<double>::infinity()
              }}
        {}
        Bbox::Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
            : mins{{
                    xmin,
                    ymin,
                    zmin
              }},
              maxes{{
                    xmax,
                    ymax,
                    zmax
              }}
        {}
        Bbox::Bbox(std::valarray<double> const& coordinates)
            : mins(coordinates), maxes(coordinates)
        {}
        Bbox::Bbox(Bbox const& other)
            : mins(other.mins), maxes(other.maxes)
        {}
        Bbox::Bbox(Bbox && other)
            : mins(std::move(other.mins)), maxes(std::move(other.maxes))
        {}
        Bbox::~Bbox(void)
        {}

        double const& Bbox::xmin(void) const noexcept
        {
            return mins[0];
        }
        double const& Bbox::ymin(void) const noexcept
        {
            return mins[1];
        }
        double const& Bbox::zmin(void) const noexcept
        {
            return mins[2];
        }
        double const& Bbox::xmax(void) const noexcept
        {
            return maxes[0];
        }
        double const& Bbox::ymax(void) const noexcept
        {
            return maxes[1];
        }
        double const& Bbox::zmax(void) const noexcept
        {
            return maxes[2];
        }

        double & Bbox::xmin(void) noexcept
        {
            return mins[0];
        }
        double & Bbox::ymin(void) noexcept
        {
            return mins[1];
        }
        double & Bbox::zmin(void) noexcept
        {
            return mins[2];
        }
        double & Bbox::xmax(void) noexcept
        {
            return maxes[0];
        }
        double & Bbox::ymax(void) noexcept
        {
            return maxes[1];
        }
        double & Bbox::zmax(void) noexcept
        {
            return maxes[2];
        }

        void Bbox::swap(Bbox & other)
        {
            using std::swap;
            swap(mins, other.mins);
            swap(maxes, other.maxes);
        }

        Bbox & Bbox::operator =(Bbox const& other) noexcept
        {
            mins = other.mins;
            maxes = other.maxes;
            return *this;
        }

        Bbox & Bbox::operator =(Bbox && other) noexcept
        {
            mins = std::move(other.mins);
            maxes = std::move(other.maxes);
            return *this;
        }

        Bbox & Bbox::operator +=(Bbox const& other)
        {
            mins[mins < other.mins] = other.mins[mins < other.mins];
            maxes[maxes > other.maxes] = other.maxes[maxes > other.maxes];
            return *this;
        }

        Bbox operator +(Bbox const& lhs, Bbox const& rhs)
        {
            Bbox result(lhs);
            result += rhs;
            return result;
        }

        CGAL::Bbox_3 Bbox::to_cgal(void) const noexcept
        {
            return CGAL::Bbox_3(mins[0], mins[1], mins[2], maxes[0], maxes[1], maxes[2]);
        }

        std::ostream & operator <<(std::ostream & os, Bbox const& bbox)
        {
            os << bbox.maxes[0] << " " << bbox.mins[0] << " " << bbox.maxes[1] << " " << bbox.mins[1] << " " << bbox.maxes[2] << " " << bbox.mins[2];
            return os;
        }

        void swap(Bbox & lhs, Bbox & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
