#include "brick_projection.h"

#include "../Algorithms/projection_algorithms.h"

#include <iterator>
#include <stdexcept>

namespace urban
{
    BrickProjection::BrickProjection(void):name("N/A"), projected_surface(), bounding_box(){}
    BrickProjection::BrickProjection(const std::string & _name, const Bbox & _bounding_box):name(_name), projected_surface(), bounding_box(Bbox_2(_bounding_box.xmin(), _bounding_box.ymin(), _bounding_box.xmax(), _bounding_box.ymax())){}
    BrickProjection::BrickProjection(const BrickProjection & other):name(other.name), facets_xy(other.facets_xy), projected_surface(other.projected_surface), bounding_box(other.bounding_box){}
    BrickProjection::~BrickProjection(void){}

    void BrickProjection::set_name(const std::string & _name)
    {
        name = _name;
    }

    void BrickProjection::set_Bbox(const Bbox & _bounding_box)
    {
        bounding_box = Bbox_2(_bounding_box.xmin(), _bounding_box.ymin(), _bounding_box.xmax(), _bounding_box.ymax());
    }


    Bbox_2 BrickProjection::bbox(void)
    {
        return bounding_box;
    }


    void BrickProjection::push_facet(FaceProjection & new_facet)
    {
        std::vector<FaceProjection> result;
        if(facets_xy.empty())
            result.push_back(new_facet);
        else
        {
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
        facets_xy = result;
    }


    bool BrickProjection::in_domain(const Point_2 & point)
    {
        return to_double(point.x()) <= bounding_box.xmax() && to_double(point.x()) >= bounding_box.xmin() && to_double(point.y()) <= bounding_box.ymax() && to_double(point.y()) <= bounding_box.ymin();
    }

    double BrickProjection::get_height(const Point_2 & point)
    {
        if(in_domain(point))
            return std::accumulate(
                std::begin(facets_xy),
                std::end(facets_xy),
                .0,
                [point](double & height, FaceProjection & facet)
                {
                    return height + facet.get_height(point);
                }
            );
        else
            throw std::out_of_range("The point is not inside the bounding box");
    }
}
