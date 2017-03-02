#include "face_projection.h"

#include "../../Algorithms/projection_algorithms.h"
#include "../../Algorithms/ogr_algorithms.h"

#include <ogr_geometry.h>

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
        FacePrint::FacePrint(const Polygon_with_holes & _border, const Plane_3 & _supporting_plane):border(_border), supporting_plane(_supporting_plane){}
        FacePrint::FacePrint(OGRFeature* ogr_facet, OGRFeatureDefn* facet_definition)
        {
            if(facet_definition->GetFieldCount() != 1)
                throw std::overflow_error("GDAL could not read the projection due to incorrect number of fields");
            InexactToExact to_exact;
            int *number_of_coefficients = NULL;
            const double * plane_coefficients = ogr_facet->GetFieldAsDoubleList("plane", number_of_coefficients);
            if(*number_of_coefficients != 4)
                throw std::overflow_error("GDAL could not read the projection due to incorrect number of plane coefficients");
            supporting_plane = Plane_3(
                to_exact(*plane_coefficients),
                to_exact(*(plane_coefficients + 1)),
                to_exact(*(plane_coefficients + 2)),
                to_exact(*(plane_coefficients + 3))
            );
            OGRGeometry* feature_polygon = ogr_facet->GetGeometryRef();
            if(feature_polygon != NULL && feature_polygon->getGeometryType() == wkbPolygon)
            {
                border = to_urban(dynamic_cast<OGRPolygon*>(feature_polygon));
            }
            else
                throw std::runtime_error("GDAL could not read a polygon from the feature");
        }
        
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
        
        Polygon FacePrint::outer_boundary(void) const
        {
            return border.outer_boundary();
        }

        Plane_3 FacePrint::get_plane(void) const noexcept
        {
            return supporting_plane;
        }

        Vector_3 FacePrint::get_normal(void) const noexcept
        {
            return Vector_3(supporting_plane.a(), supporting_plane.b(), supporting_plane.c());
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

        Bbox_2 FacePrint::bbox(void) const
        {
            return border.bbox();
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

        OGRFeature* FacePrint::to_ogr(OGRFeatureDefn* feature_definition) const
        {
            OGRFeature* feature = OGRFeature::CreateFeature(feature_definition);
            feature->SetGeometry(urban::to_ogr(border));

            double plane_coefficients[4] = {to_double(supporting_plane.a()), to_double(supporting_plane.b()), to_double(supporting_plane.c()), to_double(supporting_plane.d())};
            feature->SetField("plane", *plane_coefficients);
            return feature;
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

    double area(const projection::FacePrint & facet)
    {
        return facet.area();
    }

}
