#include "brick_projection.h"

#include "../../algorithms/projection/projection_algorithms.h"
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

        Bbox_2 BrickPrint::bbox(void) const noexcept
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
        
        bool BrickPrint::check_integrity(void) const
        {
            Polygon_set diff(projected_surface);
            for(auto const& facet : projected_facets)
            {
                diff.difference(facet.get_polygon());
            }
            return diff.is_empty();
        }

        bool BrickPrint::has_same_footprint(BrickPrint const& other) const
        {
            Polygon_set l_copy(projected_surface);
            l_copy.symmetric_difference(other.projected_surface);
            return l_copy.is_empty();
        }
        bool BrickPrint::has_same_facets(BrickPrint const& other) const
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

        std::vector<FacePrint> BrickPrint::occlusion(FacePrint const& lfacet)
        {
            std::vector<FacePrint> occluded_lfacet;
            std::vector<FacePrint> occluded_projected_facets;

            for(auto const& rfacet : projected_facets)
            {
                if(!rfacet.is_degenerate())
                {
                    Polygon_with_holes  lhs(lfacet.get_polygon()),
                                        rhs(rfacet.get_polygon());
                    std::vector<Polygon_with_holes> intersections;
                    CGAL::intersection(lhs, rhs, std::back_inserter(intersections));
                    if(intersections.empty())
                    {
                        occluded_lfacet.push_back(lfacet);
                        occluded_projected_facets.push_back(rfacet);
                    }
                    else
                    {
                        Polygon_set _lhs,
                                    _rhs;
                        
                        for(auto const& intersection : intersections)
                        {
                            Point_2 sample_point = CGAL::centroid(intersection.outer_boundary()[0], intersection.outer_boundary()[1], intersection.outer_boundary()[2]);

                            if(lfacet.get_plane_height(sample_point) > rfacet.get_plane_height(sample_point))
                                _rhs.join(intersection);
                            else
                                _lhs.join(intersection);
                        }
                        _lhs.complement().intersection(lhs);
                        _rhs.complement().intersection(rhs);

                        occluded_lfacet = add(occluded_lfacet, _lhs, lfacet.get_plane());
                        occluded_projected_facets = add(occluded_projected_facets, _rhs, rfacet.get_plane());
                    }
                }
            }

            projected_facets = occluded_projected_facets;
            return occluded_lfacet;
        }

        Brickprint & BrickPrint::occlusion(Brickprint & other)
        {
            std::vector<FacePrint> result;
            for(auto const& facet : other.projected_facets)
            {
                auto ofacet = occlusion(facet);
                result.insert(std::end(result), std::begin(ofacet), std::end(ofacet));
            }
            other.projected_surface = result;
            return other;
        }

        BrickPrint & BrickPrint::operator +=(BrickPrint const& other)
        {
            bounding_box += other.bounding_box;

            other = occlusion(other);
            projected_facets.insert(std::end(projected_facets), std::begin(other.projected_facets), std::end(other.projected_facets));

            return *this;
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

        BrickPrint & operator +(BrickPrint & lhs, BrickPrint & rhs)
        {
            return lhs += rhs;
        }

        bool operator ==(BrickPrint const& lhs, BrickPrint const& rhs)
        {
            return  lhs.epsg_index == rhs.epsg_index
                    &&
                    lhs.reference_point == rhs.reference_point
                    &&
                    lhs.has_same_footprint(rhs)
                    &&
                    lhs.has_same_facets(rhs);
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
