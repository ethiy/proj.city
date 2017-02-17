#include "bbox.h"

#include <algorithm>

#include <limits>

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
            swap(extreemes, other.extreemes)
        }

        Bbox & operator=(const Bbox & other) noexcept
        {
            extreemes = other.extreemes;
            return *this;
        }

        Bbox & Bbox::operator=(Bbox && other) noexcept
        {
            extreemes = std::move(other.extreemes);
            return *this;
        }

        Bbox & operator+=(const Bbox & other)
        {
            extreems.at(0) = std::min<double>(extreems.at(0), other.extreems.at(0));
            extreems.at(1) = std::max<double>(extreems.at(1), other.extreems.at(1));
            extreems.at(2) = std::min<double>(extreems.at(2), other.extreems.at(2));
            extreems.at(3) = std::max<double>(extreems.at(3), other.extreems.at(3));
            extreems.at(4) = std::min<double>(extreems.at(4), other.extreems.at(4));
            extreems.at(5) = std::max<double>(extreems.at(5), other.extreems.at(5));
            return *this;
        }

        void swap(Bbox & lhs, Bbox & rhs)
        {
            lhs.swap(rhs);
        }

        Bbox & operator+(Bbox & lhs, const Bbox & rhs)
        {
            return lhs += rhs;
        }

        std::ostream & operator<<(std::ostream & os, Bbox & bbox)
        {
            os << bbox.xmin() << " " << bbox.xmax() << " " << bbox.ymin() << " " << bbox.ymax() << " " << bbox.zmin() << " " << bbox.zmax();
            return os;
        }
    }
}
