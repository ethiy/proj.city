#include "brick_projection.h"

#include "../Algorithms/projection_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <list>
#include <iterator>

#include <stdexcept>

namespace urban
{
    BrickProjection::BrickProjection(void):name("N/A"), projected_surface(), bounding_box(){}
    BrickProjection::BrickProjection(const std::string & _name, const Bbox & _bounding_box):name(_name), projected_surface(), bounding_box(Bbox_2(_bounding_box.xmin(), _bounding_box.ymin(), _bounding_box.xmax(), _bounding_box.ymax())){}
    BrickProjection::BrickProjection(const BrickProjection & other):name(other.name), projected_facets(other.projected_facets), projected_surface(other.projected_surface), bounding_box(other.bounding_box){}
    BrickProjection::BrickProjection(BrickProjection && other):name(std::move(other.name)), projected_facets(std::move(other.projected_facets)), projected_surface(std::move(other.projected_surface)), bounding_box(std::move(other.bounding_box)){}
    BrickProjection::~BrickProjection(void){}

    void BrickProjection::swap(BrickProjection & other)
    {
        using std::swap;
        swap(name, other.name);
        swap(projected_facets, other.projected_facets);
        swap(projected_surface, other.projected_surface);
        swap(bounding_box, other.bounding_box);
    }
        
    BrickProjection BrickProjection::operator=(const BrickProjection & other)
    {
        name = other.name;
        projected_facets = std::move(other.projected_facets);
        projected_surface = std::move(other.projected_surface);
        bounding_box = std::move(other.bounding_box);
        return *this;
    }

    BrickProjection BrickProjection::operator=(BrickProjection && other)
    {
        name = std::move(other.name);
        projected_facets.resize(other.projected_facets.size());
        std::copy(std::begin(other.projected_facets), std::end(other.projected_facets), std::begin(projected_facets));
        projected_surface = std::move(other.projected_surface);
        bounding_box = std::move(other.bounding_box);
        return *this;
    }


    Bbox_2 BrickProjection::bbox(void)
    {
        return bounding_box;
    }


    bool BrickProjection::contains(const Polygon_with_holes & facet) const
    {
        std::list<Polygon_with_holes> _inter(0);
        if(CGAL::do_overlap(projected_surface.bbox(), facet.bbox()))
            CGAL::intersection(projected_surface, facet, std::back_inserter(_inter));
        return _inter.size() == 1 && _inter.front() == facet;
    }

    bool BrickProjection::is_under(const FaceProjection & facet) const
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
            under = contains(facet.get_polygon()) && facet.get_height(point) < get_height(point);
        }
        return under;
    }
    

    void BrickProjection::push_facet(FaceProjection & new_facet)
    {
        std::list<FaceProjection> result(0);
        if(projected_facets.empty())
        {
            if(projected_surface.outer_boundary().is_empty())
                result.push_back(new_facet);
            else
                std::logic_error("Something went wrong! The projected surface should be an accumulation of all xy-projected facets");
        }
        else
        {
            /* If new_facet is under the surface we loose it*/
            if(!is_under(new_facet))
            // {
            //     std::vector< std::list<FaceProjection> > occlusion_results(projected_facets.size());
            //     std::transform(
            //         std::begin(projected_facets),
            //         std::end(projected_facets),
            //         std::begin(occlusion_results),
            //         [&new_facet](FaceProjection & facet)
            //         {
            //             return occlusion(facet, new_facet);
            //         }
            //     );
            //     result = std::accumulate(
            //         std::begin(occlusion_results),
            //         std::end(occlusion_results),
            //         result,
            //         [](std::list<FaceProjection> & concat_result, std::list<FaceProjection> & occlusion_result)
            //         {
            //             return concat_result.splice(std::end(concat_result), occlusion_result);
            //         }
            //     );
            // }
                std::for_each(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    [&result, &new_facet](FaceProjection & facet)
                    {
                        std::list<FaceProjection> occlusion_result(occlusion(facet, new_facet));
                        result.splice(std::end(result), occlusion_result);

                    }
                );
        }
        projected_facets = std::move(result);
    }


    bool BrickProjection::in_domain(const Point_2 & point) const
    {
        return point.x() <= bounding_box.xmax() && point.x() >= bounding_box.xmin() && point.y() <= bounding_box.ymax() && point.y() <= bounding_box.ymin();
    }

    double BrickProjection::get_height(const Point_2 & point) const
    {
        if(true) // To be checked
            return std::accumulate(
                std::begin(projected_facets),
                std::end(projected_facets),
                .0,
                [point](double & height, const FaceProjection & facet)
                {
                    return height + facet.get_height(point);
                }
            );
        else
            throw std::out_of_range("The point is not inside the bounding box");
    }

    void swap(BrickProjection & lhs, BrickProjection & rhs)
    {
        lhs.swap(rhs);
    }
}
