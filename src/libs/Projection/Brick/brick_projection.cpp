#include "brick_projection.h"

#include "../../Algorithms/projection_algorithms.h"

#include <CGAL/Boolean_set_operations_2.h>

#include <list>
#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    namespace projection
    {
        BrickPrint::BrickPrint(void):name("N/A"), projected_surface(), bounding_box(){}
        BrickPrint::BrickPrint(const std::string & _name, const Bbox_3 & _bounding_box):name(_name + "_projected_xy"), bounding_box(Bbox_2(_bounding_box.xmin(), _bounding_box.ymin(), _bounding_box.xmax(), _bounding_box.ymax())){}
        BrickPrint::BrickPrint(const std::string & _name, OGRLayer* projection_layer): name(_name)
        {
            projection_layer->ResetReading();

            OGRFeature* ogr_facet = NULL;
            while((ogr_facet = projection_layer->GetNextFeature()) != NULL)
            {
                FacePrint facet(ogr_facet, projection_layer->GetLayerDefn());
                projected_facets.push_back(facet);
                OGRFeature::DestroyFeature(ogr_facet);
                projected_surface.join(facet.get_polygon());
                bounding_box += facet.bbox();
            }
            OGRFeature::DestroyFeature(ogr_facet);
        }

        BrickPrint::BrickPrint(const FacePrint & face_projection):name("contains_only_one_facet"), projected_facets(std::list<FacePrint>{{face_projection}}), projected_surface(Polygon_set(face_projection.get_polygon())) , bounding_box(face_projection.bbox()) {}
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
            
        BrickPrint & BrickPrint::operator=(const BrickPrint & other)
        {
            name = other.name;
            projected_facets = std::move(other.projected_facets);
            projected_surface = std::move(other.projected_surface);
            bounding_box = std::move(other.bounding_box);
            return *this;
        }

        BrickPrint & BrickPrint::operator=(BrickPrint && other)
        {
            name = std::move(other.name);
            projected_facets.resize(other.projected_facets.size());
            std::copy(std::begin(other.projected_facets), std::end(other.projected_facets), std::begin(projected_facets));
            projected_surface = std::move(other.projected_surface);
            bounding_box = std::move(other.bounding_box);
            return *this;
        }

        BrickPrint & BrickPrint::operator+=(const BrickPrint & other)
        {
            name += "_" + other.name;
            if(projected_surface.do_intersect(other.projected_surface))
            {
                std::for_each(
                    std::begin(other.projected_facets),
                    std::end(other.projected_facets),
                    [this](const FacePrint & facet)
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


        bool BrickPrint::contains(const FacePrint & facet) const
        {
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(facet.get_polygon());
            std::list<Polygon_with_holes> _inter;
            shallow_copy.polygons_with_holes(std::back_inserter(_inter));
            return _inter.size() == 1 && _inter.front() == facet.get_polygon();
        }

        bool BrickPrint::overlaps(const FacePrint & facet) const
        {
            Polygon_set shallow_copy(projected_surface);
            shallow_copy.intersection(facet.get_polygon());
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
                [&diff](const FacePrint & facet)
                {
                    return diff.difference(facet.get_polygon());
                }
            );
            return diff.is_empty();
        }

        void BrickPrint::insert(const FacePrint & new_facet)
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


        bool BrickPrint::in_domain(const Point_2 & point) const
        {
            return bounding_box.xmax() - point.x() > std::numeric_limits<double>::epsilon()
                    &&
                   point.x() - bounding_box.xmin() > std::numeric_limits<double>::epsilon()
                    &&
                   bounding_box.ymax() - point.y() > std::numeric_limits<double>::epsilon()
                    &&
                   point.y() - bounding_box.ymin() > std::numeric_limits<double>::epsilon();
        }

        double BrickPrint::get_height(const Point_2 & point) const
        {
            if(in_domain(point))
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

        void BrickPrint::to_ogr(GDALDataset* file) const
        {
            OGRSpatialReference spatial_reference_system;
            spatial_reference_system.importFromEPSG(
                epsg_index
            );
            OGRLayer* projection_layer = file->CreateLayer("projection_xy", &spatial_reference_system, wkbPolygon, NULL);
            if(projection_layer == NULL)
                throw std::runtime_error("GDAL could not create a projection layer");
            int width(static_cast<int>(projected_facets.size()));
            OGRFieldDefn plane_coefficient_a("Plane coefficient a", OFTReal);
            plane_coefficient_a.SetWidth(width);
            OGRFieldDefn plane_coefficient_b("Plane coefficient b", OFTReal);
            plane_coefficient_a.SetWidth(width);
            OGRFieldDefn plane_coefficient_c("Plane coefficient c", OFTReal);
            plane_coefficient_a.SetWidth(width);
            OGRFieldDefn plane_coefficient_d("Plane coefficient d", OFTReal);
            plane_coefficient_a.SetWidth(width);

            if(
               (projection_layer->CreateField(&plane_coefficient_a) != OGRERR_NONE ) && (projection_layer->CreateField(&plane_coefficient_b) != OGRERR_NONE )
                &&
               (projection_layer->CreateField(&plane_coefficient_c) != OGRERR_NONE ) && (projection_layer->CreateField(&plane_coefficient_d) != OGRERR_NONE )
              )
                    throw std::runtime_error("GDAL could not create plane coefficient fields");
                
            std::for_each(
                std::begin(projected_facets),
                std::end(projected_facets),
                [projection_layer](const projection::FacePrint & facet)
                {
                    OGRFeature* ogr_facet = facet.to_ogr(projection_layer->GetLayerDefn());
                    if(projection_layer->CreateFeature(ogr_facet) != OGRERR_NONE)
                        throw std::runtime_error("GDAL could not insert the facet in shapefile");
                    OGRFeature::DestroyFeature(ogr_facet);
                }
            );
        }

        std::ostream & operator<<(std::ostream & os, const BrickPrint & brick_projection)
        {
            os << "Name: " << brick_projection.name << std::endl
               << "Bounding box: " << brick_projection.bounding_box << std::endl
               << "Face Projections: " << brick_projection.projected_facets.size() << std::endl;
            std::copy(std::begin(brick_projection.projected_facets), std::end(brick_projection.projected_facets), std::ostream_iterator<FacePrint>(os, "\n"));
            std::vector<Polygon_with_holes> copy;
            brick_projection.projected_surface.polygons_with_holes(std::back_inserter(copy));
            os << "Projected surface: " << std::endl;
            std::copy(std::begin(copy), std::end(copy), std::ostream_iterator<Polygon_with_holes>(os, "\n"));
            return os;
        }

        BrickPrint & operator+(BrickPrint & lhs, const BrickPrint & rhs)
        {
            return lhs += rhs;
        }
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
