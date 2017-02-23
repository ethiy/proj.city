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
        : extreemes{{
            std::numeric_limits<double>::infinity(),
            - std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::infinity(),
            - std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::infinity(),
            - std::numeric_limits<double>::infinity()
        }} {}

        Bbox::Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
        : extreemes{{
            xmin,
            xmax,
            ymin,
            ymax,
            zmin,
            zmax
        }} {}

        Bbox::Bbox(const std::array<double, 3> & coordinates)
        : extreemes{{
            coordinates.at(0),
            coordinates.at(0),
            coordinates.at(1),
            coordinates.at(1),
            coordinates.at(2),
            coordinates.at(2),
        }} {}

        Bbox::Bbox(const Bbox & other): extreemes(other.extreemes) {}

        Bbox::Bbox(Bbox && other): extreemes(std::move(other.extreemes)) {}

        Bbox::~Bbox(void){}

        double Bbox::xmin(void) const noexcept
        {
            return extreemes.at(0);
        }

        double Bbox::xmax(void) const noexcept
        {
            return extreemes.at(1);
        }
        
        double Bbox::ymin(void) const noexcept
        {
            return extreemes.at(2);
        }

        double Bbox::ymax(void) const noexcept
        {
            return extreemes.at(3);
        }

        double Bbox::zmin(void) const noexcept
        {
            return extreemes.at(4);
        }

        double Bbox::zmax(void) const noexcept
        {
            return extreemes.at(5);
        }

        void Bbox::swap(Bbox & other)
        {
            using std::swap;
            swap(extreemes, other.extreemes);
        }

        Bbox & Bbox::operator=(const Bbox & other) noexcept
        {
            extreemes = other.extreemes;
            return *this;
        }

        Bbox & Bbox::operator=(Bbox && other) noexcept
        {
            extreemes = std::move(other.extreemes);
            return *this;
        }

        Bbox & Bbox::operator+=(const Bbox & other)
        {
            extreemes.at(0) = std::min<double>(extreemes.at(0), other.extreemes.at(0));
            extreemes.at(1) = std::max<double>(extreemes.at(1), other.extreemes.at(1));
            extreemes.at(2) = std::min<double>(extreemes.at(2), other.extreemes.at(2));
            extreemes.at(3) = std::max<double>(extreemes.at(3), other.extreemes.at(3));
            extreemes.at(4) = std::min<double>(extreemes.at(4), other.extreemes.at(4));
            extreemes.at(5) = std::max<double>(extreemes.at(5), other.extreemes.at(5));
            return *this;
        }

        Bbox operator+(const Bbox & lhs, const Bbox & rhs)
        {
            Bbox copy(lhs);
            copy += rhs;
            return copy;
        }

        std::ostream & operator<<(std::ostream & os, const Bbox & bbox)
        {
            os << bbox.extreemes.at(0) << " " << bbox.extreemes.at(1) << " " << bbox.extreemes.at(2) << " " << bbox.extreemes.at(3) << " " << bbox.extreemes.at(4) << " " << bbox.extreemes.at(5);
            return os;
        }
    }

    void swap(shadow::Bbox & lhs, shadow::Bbox & rhs)
    {
        lhs.swap(rhs);
    }

}
