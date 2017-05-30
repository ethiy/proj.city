#include "face_projection.h"

#include "../../algorithms/projection/projection_algorithms.h"
#include "../Raster/raster_projection.h"
#include "../../algorithms/ogr/ogr_algorithms.h"
#include "../../algorithms/utils/util_algorithms.h"

#include <ogr_geometry.h>

#include <CGAL/Boolean_set_operations_2.h>

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
        FacePrint::FacePrint(Polygon_with_holes const& _border, const Plane_3 & _supporting_plane):border(_border), supporting_plane(_supporting_plane){}
        FacePrint::FacePrint(OGRFeature* ogr_facet, OGRFeatureDefn* facet_definition)
        {
            if(facet_definition->GetFieldCount() < 4)
                throw std::overflow_error("GDAL could not read the projection due to incorrect number of fields");
            InexactToExact to_exact;
            supporting_plane = Plane_3(
                to_exact(ogr_facet->GetFieldAsDouble("coeff_a")),
                to_exact(ogr_facet->GetFieldAsDouble("coeff_b")),
                to_exact(ogr_facet->GetFieldAsDouble("coeff_c")),
                to_exact(ogr_facet->GetFieldAsDouble("coeff_d"))
            );
            OGRGeometry* feature_polygon = ogr_facet->GetGeometryRef();
            if(feature_polygon != NULL && feature_polygon->getGeometryType() == wkbPolygon)
            {
                border = get_ogr_polygon(dynamic_cast<OGRPolygon*>(feature_polygon));
            }
            else
                throw std::runtime_error("GDAL could not read a polygon from the feature");
        }
        
        FacePrint::FacePrint(FacePrint const& other):border(other.border), supporting_plane(other.supporting_plane){}
        FacePrint::FacePrint(FacePrint && other):border(std::move(other.border)), supporting_plane(std::move(other.supporting_plane)){}
        FacePrint::~FacePrint(void){}

        void FacePrint::swap(FacePrint & other)
        {
            using std::swap;
            swap(border, other.border);
            swap(supporting_plane, other.supporting_plane);
        }

        FacePrint & FacePrint::operator =(FacePrint const& other) noexcept
        {
            border = other.border;
            supporting_plane = other.supporting_plane;
            return *this;
        }
        FacePrint & FacePrint::operator =(FacePrint && other) noexcept
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

        double FacePrint::get_plane_height(Point_2 const& point) const
        {
            if( supporting_plane.c() == 0)
                throw std::overflow_error("The supporting plane is vertical!");
            return to_double(( -1 * supporting_plane.d() - supporting_plane.a() * point.x() - supporting_plane.b() * point.y()) / supporting_plane.c()) ;
        }

        double FacePrint::get_plane_height(InexactPoint_2 const& inexact_point) const
        {
            ExactToInexact to_inexact;
            if( std::abs(to_inexact(supporting_plane.c())) < std::numeric_limits<double>::epsilon() )
                throw std::overflow_error("The supporting plane is vertical!");
            return ( -1 * to_inexact(supporting_plane.d()) - to_inexact(supporting_plane.a()) * inexact_point.x() - to_inexact(supporting_plane.b()) * inexact_point.y()) / to_inexact(supporting_plane.c()) ;
        }


        double FacePrint::get_height(Point_2 const& point) const
        {
            return !is_degenerate() * contains(point) * get_plane_height(point) ;
        }

        double FacePrint::get_height(InexactPoint_2 const& inexact_point) const
        {
            return !is_degenerate() * contains(inexact_point) * get_plane_height(inexact_point) ;
        }

        double FacePrint::get_height(double top_left_x, double top_left_y, double pixel_size, bool & hit) const
        {
            double mean_height(0.);

            InexactToExact to_exact;
            std::vector<Point_2> pixel_corners{{
                to_exact(InexactPoint_2(top_left_x, top_left_y)),
                to_exact(InexactPoint_2(top_left_x, top_left_y - pixel_size)),
                to_exact(InexactPoint_2(top_left_x + pixel_size, top_left_y - pixel_size)),
                to_exact(InexactPoint_2(top_left_x + pixel_size, top_left_y))
            }};
            
            std::vector<Polygon_with_holes> pixel_inter;
            CGAL::intersection(
                Polygon(std::begin(pixel_corners), std::end(pixel_corners)),
                border,
                std::back_inserter(pixel_inter)
            );
            hit = !pixel_inter.empty();

            if(hit)
            {
                std::vector<double> weights(pixel_inter.size(), static_cast<double>(pixel_inter.size()));
                mean_height = std::inner_product(
                    std::begin(weights),
                    std::end(weights),
                    std::begin(pixel_inter),
                    double(0.),
                    std::plus<double>(),
                    [this](double const& parts, Polygon_with_holes const& part)
                    {
                        return get_plane_height(urban::centroid(part)) / parts;
                    }
                );
            }
            return mean_height;
        }

        InexactPoint_2 FacePrint::centroid(void) const
        {
            return urban::centroid(border.outer_boundary());
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


        RasterPrint & FacePrint::rasterize_to(RasterPrint & raster_projection, const shadow::Point & pivot) const
        {
            if(!is_degenerate())
            {
                /**
                 * 'bb(ox)' pronounced in french could mean babe alias bae.
                 */
                Bbox_2 bae = border.bbox();
                double pixel_size = raster_projection.get_pixel_size();
                std::size_t  i_min = static_cast<std::size_t>(std::floor((raster_projection.get_reference_point().y() - bae.ymax() - pivot.y()) / pixel_size)),
                             j_min = static_cast<std::size_t>(std::floor((bae.xmin() - raster_projection.get_reference_point().x() + pivot.x()) / pixel_size));
                std::size_t  w = static_cast<std::size_t>(std::ceil((bae.xmax() - bae.xmin()) / pixel_size)),
                             h = static_cast<std::size_t>(std::ceil((bae.ymax() - bae.ymin()) / pixel_size));
                if(i_min + h > raster_projection.get_height() && j_min + w > raster_projection.get_width())
                {
                    std::stringstream error_message("Oh noooz!! I iz outsidez ze box");
                    error_message << i_min + h << " > " << raster_projection.get_height() << " or " << j_min + w << " > " << raster_projection.get_width();
                    throw std::runtime_error(error_message.str());
                }
                std::vector<std::size_t> indexes(w * h);
                std::iota(std::begin(indexes), std::end(indexes), 0);
                std::for_each(
                    std::begin(indexes),
                    std::end(indexes),
                    [pixel_size, w, &bae, this, &raster_projection, i_min, j_min](std::size_t const& index)
                    {
                        bool hit(false);
                        double height = get_height(
                            bae.xmin() + static_cast<double>(index%w) * pixel_size,
                            bae.ymax() - static_cast<double>(index/w) * pixel_size,
                            pixel_size,
                            hit
                        );
                        if(hit)
                            raster_projection.at(i_min + index/w, j_min + index%w) = (raster_projection.at(i_min + index/w, j_min + index%w) * raster_projection.hit(i_min + index/w, j_min + index%w) + height) / (++raster_projection.hit(i_min + index/w, j_min + index%w));
                    }
                );
            }
            
            return raster_projection;
        }

        bool FacePrint::has_same_border(FacePrint const& other) const
        {
            bool result = std::distance(border.holes_begin(), border.holes_end()) == std::distance(other.border.holes_begin(), other.border.holes_end());
            if(result)
            {
                std::vector<bool> results(static_cast<std::size_t>(std::distance(border.holes_begin(), border.holes_end())));
                std::transform(
                    border.holes_begin(),
                    border.holes_end(),
                    other.border.holes_begin(),
                    std::begin(results),
                    [](const Polygon & l_poly, const Polygon & r_poly)
                    {
                        return l_poly == r_poly;
                    }
                );
                result = std::accumulate(
                    std::begin(results),
                    std::end(results),
                    border.outer_boundary() == other.border.outer_boundary(),
                    [](bool & all, const bool r)
                    {
                        return all && r;
                    }
                );
            }
            return result;
        }

        bool FacePrint::has_same_plane(FacePrint const& other) const
        {
            return supporting_plane == other.supporting_plane;
        }


        FacePrint::Hole_const_iterator FacePrint::holes_begin(void) const
        {
            return border.holes_begin();
        }

        FacePrint::Hole_const_iterator FacePrint::holes_end(void) const
        {
            return  border.holes_end();
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
        bool FacePrint::is_empty(void) const
        {
            return border.outer_boundary().is_empty();
        }

        bool FacePrint::contains(Point_2 const& point) const
        {
            return  border.outer_boundary().bounded_side(point) != CGAL::ON_UNBOUNDED_SIDE
                    &&
                    std::all_of(
                        border.holes_begin(),
                        border.holes_end(),
                        [&point](Polygon const& hole)
                        {
                            return hole.bounded_side(point) != CGAL::ON_BOUNDED_SIDE;
                        }
                    );
        }

        bool FacePrint::contains(InexactPoint_2 const& inexact_point) const
        {
            InexactToExact to_exact;
            return contains(to_exact(inexact_point));
        }

        OGRFeature* FacePrint::to_ogr(OGRFeatureDefn* feature_definition, const shadow::Point & reference_point) const
        {
            OGRFeature* feature = OGRFeature::CreateFeature(feature_definition);
            feature->SetGeometry(urban::to_ogr(border, reference_point));
            ExactToInexact to_inexact;
            feature->SetField("coeff_a", to_inexact(supporting_plane.a()));
            feature->SetField("coeff_b", to_inexact(supporting_plane.b()));
            feature->SetField("coeff_c", to_inexact(supporting_plane.c()));
            feature->SetField("coeff_d", to_inexact(supporting_plane.d() + reference_point.z()));
            return feature;
        }

        std::ostream & operator <<(std::ostream & os, FacePrint const& facet)
        {
            return os << "The Polygon describing borders :" << facet.border << std::endl
                      << "The supporting plane coefficients : " << facet.supporting_plane << std::endl;
        }

        bool operator ==(FacePrint const& lhs, FacePrint const& rhs)
        {
            return lhs.has_same_border(rhs) && lhs.has_same_plane(rhs);
        }

        bool operator !=(FacePrint const& lhs, FacePrint const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(projection::FacePrint & lhs, projection::FacePrint & rhs)
    {
        lhs.swap(rhs);
    }

    double area(projection::FacePrint const& facet)
    {
        return facet.area();
    }

}
