#include <projection/face_projection.h>

#include <projection/raster_projection.h>
#include <projection/utilities.h>
#include <algorithms/util_algorithms.h>

#include <ogr_feature.h>
#include <ogr_geometry.h>

#include <CGAL/Boolean_set_operations_2.h>

#include <algorithm>
#include <iterator>

#include <cmath>
#include <limits>

#include <stdexcept>

namespace city
{
    namespace projection
    {
        FacePrint::FacePrint(void)
        {}
        FacePrint::FacePrint(std::size_t const _id, Polygon_with_holes const& _border, Plane_3 const& _supporting_plane)
            : id(_id), border(_border), supporting_plane(_supporting_plane)
        {}
        FacePrint::FacePrint(::city::scene::UNode::Facet const& facet)
            : id(facet.id())
        {
            Polygon facet_proj = trace(facet, supporting_plane);

            if(facet_proj.is_simple() && facet_proj.orientation() == CGAL::CLOCKWISE)
                facet_proj.reverse_orientation();

            border = Polygon_with_holes(facet_proj);
        }
        FacePrint::FacePrint(OGRFeature* ogr_facet, OGRFeatureDefn* facet_definition)
        {
            if(facet_definition->GetFieldCount() < 5)
                throw std::overflow_error("GDAL could not read the projection due to incorrect number of fields");
            
            id = static_cast<std::size_t>(ogr_facet->GetFieldAsInteger64("Id"));

            InexactToExact to_exact;
            supporting_plane = Plane_3(
                to_exact(ogr_facet->GetFieldAsDouble("coeff_a")),
                to_exact(ogr_facet->GetFieldAsDouble("coeff_b")),
                to_exact(ogr_facet->GetFieldAsDouble("coeff_c")),
                to_exact(ogr_facet->GetFieldAsDouble("coeff_d"))
            );

            OGRGeometry* feature_polygon = ogr_facet->GetGeometryRef();
            if(feature_polygon != nullptr && feature_polygon->getGeometryType() == wkbPolygon)
                border = get_ogr_polygon(dynamic_cast<OGRPolygon*>(feature_polygon));
            else
                throw std::runtime_error("GDAL could not read a polygon from the feature");
        }
        FacePrint::FacePrint(FacePrint const& other)
            : id(other.id), border(other.border), supporting_plane(other.supporting_plane)
        {}
        FacePrint::FacePrint(FacePrint && other)
            : id(std::move(other.id)), border(std::move(other.border)), supporting_plane(std::move(other.supporting_plane))
        {}
        FacePrint::~FacePrint(void)
        {}

        void FacePrint::swap(FacePrint & other)
        {
            using std::swap;
            swap(id, other.id);
            swap(border, other.border);
            swap(supporting_plane, other.supporting_plane);
        }

        FacePrint & FacePrint::operator =(FacePrint const& other) noexcept
        {
            id = other.id;
            border = other.border;
            supporting_plane = other.supporting_plane;
            return *this;
        }
        FacePrint & FacePrint::operator =(FacePrint && other) noexcept
        {
            id = std::move(other.id);
            border = std::move(other.border);
            supporting_plane = std::move(other.supporting_plane);
            return *this;
        }

        std::size_t FacePrint::get_id() const noexcept
        {
            return id;
        }        
        Polygon_with_holes const& FacePrint::get_polygon(void) const noexcept
        {
            return border;
        }
        Polygon const& FacePrint::outer_boundary(void) const
        {
            return border.outer_boundary();
        }
        Plane_3 const& FacePrint::get_plane(void) const noexcept
        {
            return supporting_plane;
        }
        Vector_3 FacePrint::get_normal(void) const noexcept
        {
            return Vector_3(supporting_plane.a(), supporting_plane.b(), supporting_plane.c());
        }
        Bbox_2 FacePrint::bbox(void) const
        {
            return border.bbox();
        }

        CGAL::Orientation FacePrint::orientation(void) const
        {
            return border.outer_boundary().orientation();
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

        std::vector<Polygon_with_holes> FacePrint::pixel_intersection(double const top_left_x, double const top_left_y, double const pixel_size, bool & hit) const
        {
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
            return pixel_inter;
        }

        double FacePrint::get_height(double top_left_x, double top_left_y, double pixel_size, bool & hit) const
        {
            double mean_height(0.);
            auto pixel_inter = pixel_intersection(top_left_x, top_left_y, pixel_size, hit);

            if(hit)
            {
                std::vector<double> mass(pixel_inter.size(), static_cast<double>(pixel_inter.size()));
                mean_height = std::inner_product(
                    std::begin(pixel_inter),
                    std::end(pixel_inter),
                    std::begin(mass),
                    double(0.),
                    std::plus<double>(),
                    [this](Polygon_with_holes const& part, double const parts)
                    {
                        return get_plane_height(city::centroid(part)) / parts;
                    }
                );
            }
            return mean_height;
        }

        InexactPoint_2 FacePrint::centroid(void) const
        {
            return city::centroid(border.outer_boundary());
        }
        double FacePrint::area(void) const
        {
            return ::city::area(border);
        }
        double FacePrint::circumference(void) const
        {
            return ::city::circumference(border.outer_boundary());
        }
        
        bool FacePrint::equal_border(FacePrint const& other) const
        {
            bool result = std::distance(border.holes_begin(), border.holes_end()) == std::distance(other.border.holes_begin(), other.border.holes_end());
            if(result)
            {
                result = std::inner_product(
                    border.holes_begin(),
                    border.holes_end(),
                    other.border.holes_begin(),
                    border.outer_boundary() == other.border.outer_boundary(),
                    std::logical_and<bool>(),
                    std::equal_to<Polygon>()
                );
            }
            return result;
        }
        bool FacePrint::equal_plane(FacePrint const& other) const
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
        bool FacePrint::empty(void) const
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

        OGRFeature* FacePrint::to_ogr(OGRFeatureDefn* feature_definition, shadow::Point const& reference_point, bool labels) const
        {
            OGRFeature* feature = OGRFeature::CreateFeature(feature_definition);

            feature->SetField("Id", static_cast<GIntBig>(id));

            feature->SetGeometry(::city::projection::to_ogr(border, reference_point));

            ExactToInexact to_inexact;
            feature->SetField("coeff_a", to_inexact(supporting_plane.a()));
            feature->SetField("coeff_b", to_inexact(supporting_plane.b()));
            feature->SetField("coeff_c", to_inexact(supporting_plane.c()));
            feature->SetField("coeff_d", to_inexact(supporting_plane.d() + reference_point.z()));
            
            if(labels)
            {
                feature->SetField("Unq_Errors", "None");
                feature->SetField("Bul_Errors", "None");
                feature->SetField("Fac_Errors", "None");
            }
            return feature;
        }

        std::vector<double> & FacePrint::rasterize(std::vector<double> & image, std::vector<short> & hits, shadow::Point const& top_left, std::size_t const height, std::size_t const width, double const pixel_size) const
        {
            if(!is_degenerate())
            {
                Bbox_2 bb = bbox();
                std::size_t  i_min = static_cast<std::size_t>(std::floor((top_left.y() - bb.ymax()) / pixel_size)),
                             j_min = static_cast<std::size_t>(std::floor((bb.xmin() - top_left.x()) / pixel_size));
                std::size_t  w = static_cast<std::size_t>(std::ceil((bb.xmax() - bb.xmin()) / pixel_size)),
                             h = static_cast<std::size_t>(std::ceil((bb.ymax() - bb.ymin()) / pixel_size));
                if(i_min + h > height && j_min + w > width)
                {
                    std::stringstream error_message("Face out of bounds: ");
                    error_message << i_min + h << " > " << height << " or " << j_min + w << " > " << width;
                    throw std::runtime_error(error_message.str());
                }

                std::vector<std::size_t> indexes(w * h);
                std::iota(std::begin(indexes), std::end(indexes), 0);
                for(auto const& index : indexes)
                {
                    bool hit = false;
                    double z = get_height(
                        bb.xmin() + static_cast<double>(index%w) * pixel_size,
                        bb.ymax() - static_cast<double>(index/w) * pixel_size,
                        pixel_size,
                        hit
                    );
                    if(hit)
                        image.at((i_min + index/w) * width + j_min + index%w)
                        =   (
                                image.at((i_min + index/w) * width + j_min + index%w) * static_cast<double>(hits.at((i_min + index/w) * width + j_min + index%w))
                                +
                                z
                            )
                            /
                            static_cast<double>(++hits.at((i_min + index/w) * width + j_min + index%w));
                }
            }
            
            return image;
        }

        std::ostream & operator <<(std::ostream & os, FacePrint const& facet)
        {
            return os << "Id: " << facet.id << std::endl
                      << "The Polygon describing borders :" << facet.border << std::endl
                      << "The supporting plane coefficients : " << facet.supporting_plane << std::endl;
        }

        bool operator ==(FacePrint const& lhs, FacePrint const& rhs)
        {
            return lhs.equal_border(rhs) && lhs.equal_plane(rhs);
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
    double circumference(projection::FacePrint const& facet)
    {
        return facet.circumference();
    }
}
