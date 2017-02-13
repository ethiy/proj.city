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
    BrickProjection::BrickProjection(const BrickProjection & other):name(other.name), facets_xy(other.facets_xy), projected_surface(other.projected_surface), bounding_box(other.bounding_box){}
    BrickProjection::BrickProjection(BrickProjection && other):name(std::move(other.name)), facets_xy(std::move(other.facets_xy)), projected_surface(std::move(other.projected_surface)), bounding_box(std::move(other.bounding_box)){}
    BrickProjection::~BrickProjection(void){}

    void BrickProjection::swap(BrickProjection & other)
    {
        using std::swap;
        swap(name, other.name);
        swap(facets_xy, other.facets_xy);
        swap(projected_surface, other.projected_surface);
        swap(bounding_box, other.bounding_box);
    }
        
    BrickProjection BrickProjection::operator=(const BrickProjection & other)
    {
        name = other.name;
        facets_xy = std::move(other.facets_xy);
        projected_surface = std::move(other.projected_surface);
        bounding_box = std::move(other.bounding_box);
        return *this;
    }

    BrickProjection BrickProjection::operator=(BrickProjection && other)
    {
        name = std::move(other.name);
        facets_xy.resize(other.facets_xy.size());
        std::copy(std::begin(other.facets_xy), std::end(other.facets_xy), std::begin(facets_xy));
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
        std::list<Polygon_with_holes> _inter;
        CGAL::intersection(projected_surface, facet, std::back_inserter(_inter));
        return _inter.size() == 1 && _inter.front() == facet;
    }

    bool BrickProjection::is_under(const FaceProjection & facet) const
    {
        Point_2 point(
            CGAL::centroid(
                facet.outer_boundary()[0],
                facet.outer_boundary()[1],
                facet.outer_boundary()[2]
            )
        );
        return contains(facet.get_polygon()) && facet.get_height(point) < get_height(point);
    }
    

    void BrickProjection::push_facet(FaceProjection & new_facet)
    {
        std::vector<FaceProjection> result;
        if(facets_xy.empty())
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
                std::for_each(
                    std::begin(facets_xy),
                    std::end(facets_xy),
                    [&result, &new_facet](FaceProjection & facet)
                    {
                        std::vector<FaceProjection> occlusion_result(occlusion(facet, new_facet));
                        result.insert(std::end(result), std::begin(occlusion_result), std::end(occlusion_result));

                    }
                );
        }
        facets_xy = std::move(result);
    }


    bool BrickProjection::in_domain(const Point_2 & point) const
    {
        return point.x() <= bounding_box.xmax() && point.x() >= bounding_box.xmin() && point.y() <= bounding_box.ymax() && point.y() <= bounding_box.ymin();
    }

    double BrickProjection::get_height(const Point_2 & point) const
    {
        if(true) // To be checked
            return std::accumulate(
                std::begin(facets_xy),
                std::end(facets_xy),
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
