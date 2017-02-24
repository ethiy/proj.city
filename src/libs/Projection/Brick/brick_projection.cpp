#include "brick_projection.h"

#include "../../Algorithms/projection_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <Python.h>

#include <list>
#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    namespace projection
    {
        BrickPrint::BrickPrint(void):name("N/A"), projected_surface() {}
        BrickPrint::BrickPrint(const std::string & _name):name(_name + "_projected_xy") {}
        BrickPrint::BrickPrint(const BrickPrint & other):name(other.name), projected_facets(other.projected_facets), projected_surface(other.projected_surface) {}
        BrickPrint::BrickPrint(BrickPrint && other):name(std::move(other.name)), projected_facets(std::move(other.projected_facets)), projected_surface(std::move(other.projected_surface)) {}
        BrickPrint::~BrickPrint(void){}

        void BrickPrint::swap(BrickPrint & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(projected_facets, other.projected_facets);
            swap(projected_surface, other.projected_surface);
        }
            
        BrickPrint BrickPrint::operator=(const BrickPrint & other)
        {
            name = other.name;
            projected_facets = std::move(other.projected_facets);
            projected_surface = std::move(other.projected_surface);
            return *this;
        }

        BrickPrint BrickPrint::operator=(BrickPrint && other)
        {
            name = std::move(other.name);
            projected_facets.resize(other.projected_facets.size());
            std::copy(std::begin(other.projected_facets), std::end(other.projected_facets), std::begin(projected_facets));
            projected_surface = std::move(other.projected_surface);
            return *this;
        }


        Bbox_2 BrickPrint::bbox(void) const
        {
            Bbox_2 BB;
            if(!projected_facets.empty())
            {
                BB = Bbox_2(projected_facets.begin()->bbox());
                BB = std::accumulate(
                    std::next(std::begin(projected_facets), 1),
                    std::end(projected_facets),
                    BB,
                    [](Bbox_2 & result, const FacePrint & facet)
                    {
                        return result + facet.bbox();
                    }
                );
            }
            
            return BB;
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
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(facet);
            std::list<Polygon_with_holes> _inter;
            shallow_copy.polygons_with_holes(std::back_inserter(_inter));
            return _inter.size() == 1 && _inter.front() == facet;
        }

        bool BrickPrint::overlaps(const Polygon_with_holes & facet) const
        {
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(facet);
            return !shallow_copy.is_empty();
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
                under = contains(facet.get_polygon()) && facet.get_height(point) < get_height(point); // there are no intersections
            }
            return under;
        }
        

        void BrickPrint::push_facet(FacePrint & new_facet)
        {
            std::list<FacePrint> result(0);
            if(projected_facets.empty())
            {
                if(projected_surface.is_empty())
                    result.push_back(new_facet);
                else
                    std::logic_error("Something went wrong! The projected surface should be an accumulation of all xy-projected facets");
            }
            else
            {
                /* If new_facet is under the surface we loose it*/
                if(!is_under(new_facet))
                {
                    projected_surface.join(new_facet.get_polygon());
                    /* If new_facet does not intersect the surface we push it directly*/
                    if(!overlaps(new_facet.get_polygon()))
                    {
                        result.push_back(new_facet);
                    }
                    else
                    {
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
                    }
                }
            }
            projected_facets = std::move(result);
        }


        bool BrickPrint::in_domain(const Point_2 & point) const
        {
            Bbox_2 bounding_box(bbox());
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

        void BrickPrint::plot(void) const
        {
            std::for_each(
                std::begin(projected_facets),
                std::end(projected_facets),
                [](const FacePrint & facet)
                {
                    PyObject* pFace = facet.plot();
                    Py_DECREF(pFace);
                }
            );
        }

        std::ostream & operator<<(std::ostream & os, const BrickPrint & brick_projection)
        {
            os << "Name: " << brick_projection.name << std::endl
               << "Bounding box: " << brick_projection.bbox() << std::endl
               << "Face Projections: " << brick_projection.projected_facets.size() << std::endl;
            std::copy(std::begin(brick_projection.projected_facets), std::end(brick_projection.projected_facets), std::ostream_iterator<FacePrint>(os, "\n"));
            std::vector<Polygon_with_holes> copy;
            brick_projection.projected_surface.polygons_with_holes(std::back_inserter(copy));
            os << "Projected surface: " << std::endl;
            std::copy(std::begin(copy), std::end(copy), std::ostream_iterator<Polygon_with_holes>(os, "\n"));
            return os;
        }
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
