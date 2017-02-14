#include "face_projection.h"

#include "../Algorithms/projection_algorithms.h"

#include <algorithm>
#include <iterator>

#include <cmath>
#include <limits>

#include <stdexcept>

namespace urban
{
    namespace projection
    {
        FacePrint::FacePrint(void){}
        FacePrint::FacePrint(const Polygon_with_holes & _border, const Plane & _supporting_plane):border(_border), supporting_plane(_supporting_plane){}
        FacePrint::FacePrint(const FacePrint & other):border(other.border), supporting_plane(other.supporting_plane){}
        FacePrint::FacePrint(FacePrint && other):border(std::move(other.border)), supporting_plane(std::move(other.supporting_plane)){}
        FacePrint::~FacePrint(void){}

        void FacePrint::swap(FacePrint & other)
        {
            using std::swap;
            swap(border, other.border);
            swap(supporting_plane, other.supporting_plane);
        }

        FacePrint & FacePrint::operator=(const FacePrint & other)
        {
            border = other.border;
            supporting_plane = other.supporting_plane;
            return *this;
        }

        FacePrint & FacePrint::operator=(FacePrint && other)
        {
            border = std::move(other.border);
            supporting_plane = std::move(other.supporting_plane);
            return *this;
        }

        Polygon_with_holes FacePrint::get_polygon(void) const noexcept
        {
            return border;
        }

        Plane FacePrint::get_plane(void) const noexcept
        {
            return supporting_plane;
        }

        Vector FacePrint::get_normal(void) const noexcept
        {
            return Vector(supporting_plane.a(), supporting_plane.b(), supporting_plane.c());
        }

        double FacePrint::get_plane_height(const Point_2 & point) const
        {
            if( supporting_plane.c() == 0)
                throw std::overflow_error("The supporting plane is vertical!");
            return to_double(( -1 * supporting_plane.d() - supporting_plane.a() * point.x() - supporting_plane.b() * point.y()) / supporting_plane.c()) ;
        }


        double FacePrint::get_height(const Point_2 & point) const
        {
            return !is_degenerate() * contains(point) * get_plane_height(point) ;
        }

        double FacePrint::area(void) const
        {
            return std::accumulate(
                        border.holes_begin(),
                        border.holes_end(),
                        to_double(border.outer_boundary().area()),
                        [](double & holes_area, const Polygon & hole)
                        {
                            return holes_area - to_double(hole.area());
                        }
                    );
        }

        FacePrint::Hole_const_iterator FacePrint::holes_begin(void) const
        {
            return border.holes_begin();
        }

        FacePrint::Hole_const_iterator FacePrint::holes_end(void) const
        {
            return  border.holes_end();
        }

        Polygon FacePrint::outer_boundary(void) const
        {
            return border.outer_boundary();
        }


        bool FacePrint::is_degenerate(void) const
        {
            /**
            * If it has no holes no need to check surface
            */
            return is_perpendicular() || ( holes_begin() != holes_end() && std::abs(area()) < std::numeric_limits<double>::epsilon() );
        }

        bool FacePrint::is_perpendicular(void) const
        {
            return supporting_plane.c() == 0;
        }

        bool FacePrint::contains(const Point_2 & point) const
        {
            return  border.outer_boundary().bounded_side(point) != CGAL::ON_UNBOUNDED_SIDE
                    &&
                    std::all_of(
                        border.holes_begin(),
                        border.holes_end(),
                        [point](Polygon hole)
                        {
                            return hole.bounded_side(point) != CGAL::ON_BOUNDED_SIDE;
                        }
                    );
        }

        std::ostream & operator<<(std::ostream & os, const FacePrint & facet)
        {
            return os << "The Polygon describing borders :" << facet.border << std::endl
                    << "The supporting plane coefficients : " << facet.supporting_plane << std::endl;
        }
    }

    void swap(projection::FacePrint & lhs, projection::FacePrint & rhs)
    {
        lhs.swap(rhs);
    }
}
