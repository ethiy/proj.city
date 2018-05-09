#include <shadow/bbox.h>

#include <shadow/point.h>

#include <algorithm>

#include <limits>
#include <iterator>
#include <algorithm>

namespace city
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
        Bbox::Bbox(Point const& point)
            : Bbox::Bbox(point.data())
        {}
        Bbox::Bbox(CGAL::Bbox_3 const& other)
            : extremes{{
                  other.xmin(),
                  other.xmax(),
                  other.ymin(),
                  other.ymax(),
                  other.zmin(),
                  other.zmax()
              }}
        {}
        Bbox::Bbox(const Bbox & other)
            : extremes(other.extremes)
        {}
        Bbox::Bbox(Bbox && other)
            : extremes(std::move(other.extremes))
        {}
        Bbox::~Bbox(void)
        {}

        double & Bbox::xmin(void) noexcept
        {
            return extremes.at(0);
        }
        double & Bbox::xmax(void) noexcept
        {
            return extremes.at(1);
        }
        double & Bbox::ymin(void) noexcept
        {
            return extremes.at(2);
        }
        double & Bbox::ymax(void) noexcept
        {
            return extremes.at(3);
        }
        double & Bbox::zmin(void) noexcept
        {
            return extremes.at(4);
        }
        double & Bbox::zmax(void) noexcept
        {
            return extremes.at(5);
        }
        double const& Bbox::xmin(void) const noexcept
        {
            return extremes.at(0);
        }
        double const& Bbox::xmax(void) const noexcept
        {
            return extremes.at(1);
        }
        double const& Bbox::ymin(void) const noexcept
        {
            return extremes.at(2);
        }
        double const& Bbox::ymax(void) const noexcept
        {
            return extremes.at(3);
        }
        double const& Bbox::zmin(void) const noexcept
        {
            return extremes.at(4);
        }
        double const& Bbox::zmax(void) const noexcept
        {
            return extremes.at(5);
        }

        void Bbox::swap(Bbox & other)
        {
            using std::swap;
            swap(extremes, other.extremes);
        }
        Bbox & Bbox::operator =(const Bbox & other) noexcept
        {
            extremes = other.extremes;
            return *this;
        }
        Bbox & Bbox::operator =(Bbox && other) noexcept
        {
            extremes = std::move(other.extremes);
            return *this;
        }

        Bbox & Bbox::operator +=(const Bbox & other)
        {
            extremes.at(0) = std::min<double>(extremes.at(0), other.extremes.at(0));
            extremes.at(1) = std::max<double>(extremes.at(1), other.extremes.at(1));
            extremes.at(2) = std::min<double>(extremes.at(2), other.extremes.at(2));
            extremes.at(3) = std::max<double>(extremes.at(3), other.extremes.at(3));
            extremes.at(4) = std::min<double>(extremes.at(4), other.extremes.at(4));
            extremes.at(5) = std::max<double>(extremes.at(5), other.extremes.at(5));
            return *this;
        }

        CGAL::Bbox_3 Bbox::to_cgal(void) const noexcept
        {
            return CGAL::Bbox_3(extremes.at(0), extremes.at(2), extremes.at(4), extremes.at(1), extremes.at(3), extremes.at(5));
        }

        std::ostream & operator <<(std::ostream & os, const Bbox & bbox)
        {
            os << bbox.extremes.at(0) << " " << bbox.extremes.at(1) << " " << bbox.extremes.at(2) << " " << bbox.extremes.at(3) << " " << bbox.extremes.at(4) << " " << bbox.extremes.at(5);
            return os;
        }

        Bbox operator +(const Bbox & lhs, const Bbox & rhs)
        {
            Bbox copy(lhs);
            copy += rhs;
            return copy;
        }

        void swap(Bbox & lhs, Bbox & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
