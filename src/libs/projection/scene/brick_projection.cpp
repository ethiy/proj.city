#include "brick_projection.h"

#include "../../algorithms/ogr/ogr_algorithms.h"

#include "utils.h"

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
        {}
        BrickPrint::BrickPrint(FacePrint const& face_projection)
            : bounding_box(face_projection.bbox()),
              projected_facets(std::vector<FacePrint>{{face_projection}}),
              projected_surface(Polygon_set(face_projection.get_polygon()))
        {}
        BrickPrint::BrickPrint(OGRLayer* projection_layer)
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
            : bounding_box(other.bounding_box),
              projected_facets(other.projected_facets),
              projected_surface(other.projected_surface)
        {}
        BrickPrint::BrickPrint(BrickPrint && other)
            : bounding_box(std::move(other.bounding_box)),
              projected_facets(std::move(other.projected_facets)),
              projected_surface(std::move(other.projected_surface))
        {}
        BrickPrint::~BrickPrint(void)
        {}

        void BrickPrint::swap(BrickPrint & other)
        {
            using std::swap;

            swap(bounding_box, other.bounding_box);
            swap(projected_facets, other.projected_facets);
            swap(projected_surface, other.projected_surface);
        }
            
        BrickPrint & BrickPrint::operator =(BrickPrint const& other)
        {
            bounding_box = other.bounding_box;
            projected_facets = other.projected_facets;
            projected_surface = other.projected_surface;
            return *this;
        }

        BrickPrint & BrickPrint::operator =(BrickPrint && other)
        {
            bounding_box = std::move(other.bounding_box);
            projected_facets = std::move(other.projected_facets);
            projected_surface = std::move(other.projected_surface);
            return *this;
        }

        Bbox_2 const& BrickPrint::bbox(void) const noexcept
        {
            return bounding_box;
        }
        std::size_t BrickPrint::size(void) const noexcept
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
        BrickPrint::const_iterator BrickPrint::begin(void) const noexcept
        {
            return projected_facets.begin();
        }
        BrickPrint::const_iterator BrickPrint::end(void) const noexcept
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
            std::vector<Polygon_with_holes> _inter;
            shallow_copy.polygons_with_holes(std::back_inserter(_inter));
            return _inter.size() == 1 && _inter.at(0) == facet.get_polygon();
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

        void BrickPrint::filter(void)
        {
            projected_facets.erase(
                std::remove_if(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    [](FacePrint const& facet)
                    {
                        return facet.is_empty() || facet.is_degenerate();
                    }
                ),
                std::end(projected_facets)
            );
        }

        BrickPrint & BrickPrint::operator +=(BrickPrint const& other)
        {
            bounding_box += other.bounding_box;
            projected_surface.join(other.projected_surface);

            auto temp = occlusion(other);
            projected_facets.insert(std::end(projected_facets), std::begin(temp.projected_facets), std::end(temp.projected_facets));
            return *this;
        }

        std::vector<FacePrint> BrickPrint::occlusion(FacePrint const& lfacet)
        {
            filter();

            std::vector<FacePrint> lhs;
            std::vector<FacePrint> rhs;

            for(auto const& rfacet : projected_facets)
            {
                std::vector<Polygon_with_holes> intersections;
                CGAL::intersection(lfacet.get_polygon(), rfacet.get_polygon(), std::back_inserter(intersections));
                if(intersections.empty())
                {
                    lhs.push_back(lfacet);
                    rhs.push_back(rfacet);
                }
                else
                {
                    Polygon_set _lhs(lfacet.get_polygon()),
                                _rhs(rfacet.get_polygon());
                
                    for(auto const& intersection : intersections)
                    {
                        Point_2 sample_point = CGAL::centroid(intersection.outer_boundary()[0], intersection.outer_boundary()[1], intersection.outer_boundary()[2]);

                        if(lfacet.get_plane_height(sample_point) > rfacet.get_plane_height(sample_point))
                            _rhs.difference(intersection);
                        else
                            _lhs.difference(intersection);
                    }

                    lhs = unpack(lhs, _lhs, lfacet.get_plane());
                    rhs = unpack(rhs, _rhs, rfacet.get_plane());
                }
            }

            projected_facets = rhs;
            return lhs;
        }
        BrickPrint BrickPrint::occlusion(BrickPrint const& other)
        {
            BrickPrint result;
            for(auto const& facet : other.projected_facets)
            {
                auto ofacet = occlusion(facet);
                result.projected_facets.insert(std::end(result), std::begin(ofacet), std::end(ofacet));
            }
            return result;
        }

        void BrickPrint::to_ogr(OGRLayer* projection_layer, shadow::Point const& reference_point, bool labels) const
        {
            OGRFieldDefn* plane_coefficient_a = new OGRFieldDefn("coeff_a", OFTReal);
            projection_layer->CreateField(plane_coefficient_a);
            OGRFieldDefn* plane_coefficient_b = new OGRFieldDefn("coeff_b", OFTReal);
            projection_layer->CreateField(plane_coefficient_b);
            OGRFieldDefn* plane_coefficient_c = new OGRFieldDefn("coeff_c", OFTReal);
            projection_layer->CreateField(plane_coefficient_c);
            OGRFieldDefn* plane_coefficient_d = new OGRFieldDefn("coeff_d", OFTReal);
            projection_layer->CreateField(plane_coefficient_d);
            if(labels)
            {
                OGRFieldDefn* unqualified_errors = new OGRFieldDefn("Unq_Errors", OFTString);
                projection_layer->CreateField(unqualified_errors);
                OGRFieldDefn* building_errors = new OGRFieldDefn("Bul_Errors", OFTString);
                projection_layer->CreateField(building_errors);
                OGRFieldDefn* facets_errors = new OGRFieldDefn("Fac_Errors", OFTString);
                projection_layer->CreateField(facets_errors);
            }

            for(auto const& facet : projected_facets)
            {
                OGRFeature* ogr_facet = facet.to_ogr(projection_layer->GetLayerDefn(), reference_point, labels);
                if(projection_layer->CreateFeature(ogr_facet) != OGRERR_NONE)
                    throw std::runtime_error("GDAL could not insert the facet in vector image!");
                OGRFeature::DestroyFeature(ogr_facet);
            }
        }

        bool BrickPrint::equal_print(BrickPrint const& other) const
        {
            Polygon_set l_copy(projected_surface);
            l_copy.symmetric_difference(other.projected_surface);
            return l_copy.is_empty();
        }
        bool BrickPrint::equal_facets(BrickPrint const& other) const
        {
            bool equality(projected_facets.size() == other.projected_facets.size());
            if(equality)
            {
                equality = std::is_permutation(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    std::begin(other.projected_facets)
                );
            }
            return equality;
        }

        std::ostream & operator <<(std::ostream & os, BrickPrint const& brick_projection)
        {
            os << "Bounding box: " << brick_projection.bounding_box << std::endl
               << "Face Projections: " << brick_projection.projected_facets.size() << std::endl;
            std::copy(std::begin(brick_projection.projected_facets), std::end(brick_projection.projected_facets), std::ostream_iterator<FacePrint>(os, "\n"));
            std::vector<Polygon_with_holes> copy;
            brick_projection.projected_surface.polygons_with_holes(std::back_inserter(copy));
            os << "Projected surface: " << std::endl;
            std::copy(std::begin(copy), std::end(copy), std::ostream_iterator<Polygon_with_holes>(os, "\n"));
            return os;
        }

        BrickPrint operator +(BrickPrint const& lhs, BrickPrint const& rhs)
        {
            BrickPrint result(lhs);
            return result += rhs;
        }
        bool operator ==(BrickPrint const& lhs, BrickPrint const& rhs)
        {
            return  lhs.equal_print(rhs)
                    &&
                    lhs.equal_facets(rhs);
        }
        bool operator !=(BrickPrint const& lhs, BrickPrint const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
