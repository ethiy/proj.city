#include "brick_projection.h"

#include "../../Algorithms/projection_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <list>
#include <iterator>

#include <stdexcept>

namespace urban
{
    namespace projection
    {
        BrickPrint::BrickPrint(void):name("N/A"), projected_surface(), bounding_box(){}
        BrickPrint::BrickPrint(const std::string & _name, const Bbox & _bounding_box):name(_name), projected_surface(), bounding_box(Bbox_2(_bounding_box.xmin(), _bounding_box.ymin(), _bounding_box.xmax(), _bounding_box.ymax())){}
        BrickPrint::BrickPrint(const BrickPrint & other):name(other.name), projected_facets(other.projected_facets), projected_surface(other.projected_surface), bounding_box(other.bounding_box){}
        BrickPrint::BrickPrint(BrickPrint && other):name(std::move(other.name)), projected_facets(std::move(other.projected_facets)), projected_surface(std::move(other.projected_surface)), bounding_box(std::move(other.bounding_box)){}
        BrickPrint::~BrickPrint(void){}

        void BrickPrint::swap(BrickPrint & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(projected_facets, other.projected_facets);
            swap(projected_surface, other.projected_surface);
            swap(bounding_box, other.bounding_box);
        }
            
        BrickPrint BrickPrint::operator=(const BrickPrint & other)
        {
            name = other.name;
            projected_facets = std::move(other.projected_facets);
            projected_surface = std::move(other.projected_surface);
            bounding_box = std::move(other.bounding_box);
            return *this;
        }

        BrickPrint BrickPrint::operator=(BrickPrint && other)
        {
            name = std::move(other.name);
            projected_facets.resize(other.projected_facets.size());
            std::copy(std::begin(other.projected_facets), std::end(other.projected_facets), std::begin(projected_facets));
            projected_surface = std::move(other.projected_surface);
            bounding_box = std::move(other.bounding_box);
            return *this;
        }


        Bbox_2 BrickPrint::bbox(void)
        {
            return bounding_box;
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


        bool BrickPrint::contains(const Polygon_with_holes & facet) const
        {
            std::list<Polygon_with_holes> _inter(0);
            if(CGAL::do_overlap(projected_surface.bbox(), facet.bbox()))
                CGAL::intersection(projected_surface, facet, std::back_inserter(_inter));
            return _inter.size() == 1 && _inter.front() == facet;
        }

        bool BrickPrint::is_under(const FacePrint & facet) const
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
        

        void BrickPrint::push_facet(FacePrint & new_facet)
        {
            std::list<FacePrint> result(0);
            size_t it(0);
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
                {
                    std::cout << "Faces to be treated:" << projected_facets.size() << std::endl;
                    std::list<FacePrint> new_facets{new_facet};
                    std::for_each(
                        std::begin(projected_facets),
                        std::end(projected_facets),
                        [&result, &new_facets, &it](FacePrint & facet)
                        {
                            std::list<FacePrint> occlusion_results(occlusion(facet, new_facets));
                            result.splice(std::end(result), occlusion_results);
                            std::cout << "Treated Facet : " << it++ << std::endl;
                        }
                    );
                    result.splice(std::end(result), new_facets);
                }
            }
            projected_facets = std::move(result);
        }


        bool BrickPrint::in_domain(const Point_2 & point) const
        {
            return point.x() <= bounding_box.xmax() && point.x() >= bounding_box.xmin() && point.y() <= bounding_box.ymax() && point.y() <= bounding_box.ymin();
        }

        double BrickPrint::get_height(const Point_2 & point) const
        {
            if(true) // To be checked
                return std::accumulate(
                    std::begin(projected_facets),
                    std::end(projected_facets),
                    .0,
                    [point](double & height, const FacePrint & facet)
                    {
                        return height + facet.get_height(point);
                    }
                );
            else
                throw std::out_of_range("The point is not inside the bounding box");
        }
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
