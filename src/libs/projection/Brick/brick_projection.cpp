#include "brick_projection.h"

#include "../../algorithms/projection/projection_algorithms.h"
#include "../../algorithms/ogr/ogr_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <list>
#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    namespace projection
    {
        BrickPrint::BrickPrint(void)
            : name("no_name") {}
        BrickPrint::BrickPrint(shadow::Point const& _reference_point)
            : name("no_name"), reference_point(_reference_point) {}
        BrickPrint::BrickPrint(std::string const& _name, Bbox_3 const& _bounding_box, shadow::Point const& _reference_point, unsigned short _espg_index)
            : name(_name + "_xy"),
              bounding_box(Bbox_2(_bounding_box.xmin(), _bounding_box.ymin(), _bounding_box.xmax(), _bounding_box.ymax())),
              reference_point(_reference_point),
              espg_index(_espg_index)
              {}
        BrickPrint::BrickPrint(FacePrint const& face_projection)
            : name("contains_only_one_facet"),
              bounding_box(face_projection.bbox()),
              projected_facets(std::list<FacePrint>{{face_projection}}),
              projected_surface(Polygon_set(face_projection.get_polygon()))
              {}
        BrickPrint::BrickPrint(std::string const& _name, OGRLayer* projection_layer): name(_name)
        {
            projection_layer->ResetReading();

            OGRFeature* ogr_facet;
            while((ogr_facet = projection_layer->GetNextFeature()) != NULL)
            {
                FacePrint facet(ogr_facet, projection_layer->GetLayerDefn());
                projected_facets.push_back(facet);
                OGRFeature::DestroyFeature(ogr_facet);
                projected_surface.join(facet.get_polygon());
                bounding_box += facet.bbox();
            }
        }
        BrickPrint::BrickPrint(BrickPrint const& other)
            : name(other.name),
              bounding_box(other.bounding_box),
              reference_point(other.reference_point),
              espg_index(other.espg_index),
              projected_facets(other.projected_facets),
              projected_surface(other.projected_surface)
              {}
        BrickPrint::BrickPrint(BrickPrint && other)
            : name(std::move(other.name)),
              bounding_box(std::move(other.bounding_box)),
              reference_point(std::move(other.reference_point)),
              espg_index(std::move(other.espg_index)),
              projected_facets(std::move(other.projected_facets)),
              projected_surface(std::move(other.projected_surface))
              {}
        BrickPrint::~BrickPrint(void){}

        void BrickPrint::swap(BrickPrint & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(bounding_box, other.bounding_box);
            swap(reference_point, other.reference_point);
            swap(espg_index, other.espg_index);
            swap(projected_facets, other.projected_facets);
            swap(projected_surface, other.projected_surface);
        }
            
        BrickPrint & BrickPrint::operator=(BrickPrint const& other)
        {
            name = other.name;
            bounding_box = other.bounding_box;
            reference_point = other.reference_point;
            espg_index = other.espg_index;
            std::copy(std::begin(other.projected_facets), std::end(other.projected_facets), std::back_inserter(projected_facets));
            projected_surface = other.projected_surface;
            return *this;
        }

        BrickPrint & BrickPrint::operator=(BrickPrint && other)
        {
            name = std::move(other.name);
            bounding_box = std::move(other.bounding_box);
            reference_point = std::move(other.reference_point);
            espg_index = std::move(other.espg_index);
            std::move(std::begin(other.projected_facets), std::end(other.projected_facets), std::back_inserter(projected_facets));
            projected_surface = std::move(other.projected_surface);
            return *this;
        }

        BrickPrint & BrickPrint::operator+=(BrickPrint const& other)
        {
            if(espg_index != other.espg_index || reference_point != other.reference_point)
                throw std::logic_error("Cannot sum two brick projections with a different projection system nor a different reference point");
            name += "_" + other.name;
            if(projected_surface.do_intersect(other.projected_surface))
            {
                std::for_each(
                    std::begin(other.projected_facets),
                    std::end(other.projected_facets),
                    [this](FacePrint const& facet)
                    {
                        insert(facet);
                    }
                );
            }
            else
            {
                std::copy(std::begin(other.projected_facets), std::end(other.projected_facets), std::back_inserter(projected_facets));
            }
            projected_surface.join(other.projected_surface);
            bounding_box += other.bounding_box;
            return *this;
        }

        
        std::string BrickPrint::get_name(void) const noexcept
        {
            return name;
        }

        Bbox_2 BrickPrint::bbox(void) const noexcept
        {
            return bounding_box;
        }

        size_t BrickPrint::size(void) const noexcept
        {
            return projected_facets.size();
        }


        BrickPrint::iterator BrickPrint::begin(void) noexcept
        {
            return projected_facets.begin();
        }
        BrickPrint::iterator BrickPrint::end(void) noexcept
        {
            return projected_facets.end();
        }
        BrickPrint::const_iterator BrickPrint::cbegin(void) const noexcept
        {
            return projected_facets.cbegin();
        }
        BrickPrint::const_iterator BrickPrint::cend(void) const noexcept
        {
            return projected_facets.cend();
        }


        bool BrickPrint::contains(Point_2 const& point) const
        {
            return projected_surface.oriented_side(point) != CGAL::ON_NEGATIVE_SIDE;
        }

        bool BrickPrint::contains(InexactPoint_2 const& inexact_point) const
        {
            InexactToExact to_exact;
            return contains(to_exact(inexact_point));
        }

        bool BrickPrint::in_domain(Point_2 const& point) const
        {
            return CGAL::do_overlap(bounding_box, point.bbox());
        }

        bool BrickPrint::contains(FacePrint const& facet) const
        {
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(facet.get_polygon());
            std::list<Polygon_with_holes> _inter;
            shallow_copy.polygons_with_holes(std::back_inserter(_inter));
            return _inter.size() == 1 && _inter.front() == facet.get_polygon();
        }

        bool BrickPrint::overlaps(Polygon const& polygon) const
        {
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(polygon);
            return !shallow_copy.is_empty();
        }
        
        bool BrickPrint::overlaps(Polygon_with_holes const& polygon) const
        {
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(polygon);
            return !shallow_copy.is_empty();
        }

        bool BrickPrint::overlaps(FacePrint const& facet) const
        {
            return overlaps(facet.get_polygon());
        }

        bool BrickPrint::is_under(FacePrint const& facet) const
        {
            bool under(false);

            if(facet.is_perpendicular() || facet.is_degenerate())
                under = true;
            else
            {
                Point_2 point(
                    CGAL::centroid(
                        facet.outer_boundary()[0],
                        facet.outer_boundary()[1],
                        facet.outer_boundary()[2]
                    )
                );
                under = contains(facet) && facet.get_height(point) <= get_height(point); /** there are no intersections */
            }
            return under;
        }
        
        bool BrickPrint::check_integrity(void) const
        {
            Polygon_set diff(projected_surface);
            std::for_each(
                std::begin(projected_facets),
                std::end(projected_facets),
                [&diff](FacePrint const& facet)
                {
                    return diff.difference(facet.get_polygon());
                }
            );
            return diff.is_empty();
        }

        bool BrickPrint::has_same_footprint(BrickPrint const& other) const
        {
            Polygon_set l_copy(projected_surface),
                        r_copy(other.projected_surface);
            l_copy.symmetric_difference(r_copy);
            return l_copy.is_empty();
        }
        bool BrickPrint::has_same_facets(BrickPrint const& other) const
        {
            bool result(false);
            if(projected_facets.size() == other.projected_facets.size())
            {
                std::vector<bool> results(projected_facets.size());
                std::transform(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    std::begin(other.projected_facets),
                    std::begin(results),
                    [](FacePrint const& l_face, FacePrint const& r_face)
                    {
                        return l_face == r_face;
                    }
                );
                result = std::accumulate(
                    std::begin(results),
                    std::end(results),
                    true,
                    [](bool & all, const bool r)
                    {
                        return all && r;
                    }
                );
            }
            return result;
        }

        void BrickPrint::insert(FacePrint const& new_facet)
        {
            if(projected_facets.empty())
                projected_facets.push_back(new_facet);
            else
            {
                /* If new_facet does not intersect the surface we push it directly*/
                if(!overlaps(new_facet))
                    projected_facets.push_back(new_facet);
                else
                {
                    /* If new_facet is under the surface we loose it*/
                    if(!is_under(new_facet))
                    {
                        std::list<FacePrint> result;
                        std::list<FacePrint> new_facets{new_facet};
                        std::for_each(
                            std::begin(projected_facets),
                            std::end(projected_facets),
                            [&result, &new_facets](FacePrint & facet)
                            {
                                std::list<FacePrint> occlusion_results(occlusion(facet, new_facets));
                                result.splice(std::end(result), occlusion_results);
                            }
                        );
                        result.splice(std::end(result), new_facets);
                        projected_facets = std::move(result);
                    }
                }
            }
            projected_surface.join(new_facet.get_polygon());
        }


        double BrickPrint::get_height(Point_2 const& point) const
        {
            double height(0.);
            if(contains(point))
                height  = std::accumulate(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    height,
                    [&point](double & result_height, FacePrint const& facet)
                    {
                        return result_height + facet.get_height(point);
                    }
                );
            return height;
        }

        double BrickPrint::get_height(InexactPoint_2 const& inexact_point) const
        {
            double height(0.);
            if(contains(inexact_point))
                height  = std::accumulate(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    height,
                    [&inexact_point](double & result_height, FacePrint const& facet)
                    {
                        return result_height + facet.get_height(inexact_point);
                    }
                );
            return height;
        }

        void BrickPrint::to_ogr(GDALDataset* file) const
        {
            OGRLayer* projection_layer = file->CreateLayer("projection_xy", NULL, wkbPolygon, NULL);
            if(projection_layer == NULL)
                throw std::runtime_error("GDAL could not create a projection layer");
            
            OGRFieldDefn* plane_coefficient_a = new OGRFieldDefn("coeff_a", OFTReal);
            projection_layer->CreateField(plane_coefficient_a);
            OGRFieldDefn* plane_coefficient_b = new OGRFieldDefn("coeff_b", OFTReal);
            projection_layer->CreateField(plane_coefficient_b);
            OGRFieldDefn* plane_coefficient_c = new OGRFieldDefn("coeff_c", OFTReal);
            projection_layer->CreateField(plane_coefficient_c);
            OGRFieldDefn* plane_coefficient_d = new OGRFieldDefn("coeff_d", OFTReal);
            projection_layer->CreateField(plane_coefficient_d);
            
            std::for_each(
                std::begin(projected_facets),
                std::end(projected_facets),
                [projection_layer, this](const projection::FacePrint & facet)
                {
                    OGRFeature* ogr_facet = facet.to_ogr(projection_layer->GetLayerDefn(), reference_point);
                    if(projection_layer->CreateFeature(ogr_facet) != OGRERR_NONE)
                        throw std::runtime_error("GDAL could not insert the facet in shapefile");
                    OGRFeature::DestroyFeature(ogr_facet);
                }
            );
        }

        std::ostream & operator<<(std::ostream & os, BrickPrint const& brick_projection)
        {
            os << "Name: " << brick_projection.name << std::endl
               << "Bounding box: " << brick_projection.bounding_box << std::endl
               << "Reference Point: " << brick_projection.reference_point << std::endl
               << "ESPG index: " << brick_projection.espg_index << std::endl
               << "Face Projections: " << brick_projection.projected_facets.size() << std::endl;
            std::copy(std::begin(brick_projection.projected_facets), std::end(brick_projection.projected_facets), std::ostream_iterator<FacePrint>(os, "\n"));
            std::vector<Polygon_with_holes> copy;
            brick_projection.projected_surface.polygons_with_holes(std::back_inserter(copy));
            os << "Projected surface: " << std::endl;
            std::copy(std::begin(copy), std::end(copy), std::ostream_iterator<Polygon_with_holes>(os, "\n"));
            return os;
        }

        BrickPrint & operator+(BrickPrint & lhs, BrickPrint const& rhs)
        {
            return lhs += rhs;
        }

        bool operator==(BrickPrint const& lhs, BrickPrint const& rhs)
        {
            return  lhs.espg_index == rhs.espg_index
                    &&
                    lhs.reference_point == rhs.reference_point
                    &&
                    lhs.has_same_footprint(rhs)
                    &&
                    lhs.has_same_facets(rhs);
        }

        bool operator!=(BrickPrint const& lhs, BrickPrint const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
