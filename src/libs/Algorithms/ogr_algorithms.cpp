#include "ogr_algorithms.h"

#include <algorithm>
#include <vector>

namespace urban
{
    OGRPoint* to_ogr(const Point_2 & point)
    {
        return new OGRPoint(to_double(point.x()), to_double(point.y()));
    }

    OGRLinearRing* to_ogr(const Polygon & polygon)
    {
        OGRLinearRing* ring = new OGRLinearRing();
        std::for_each(
            polygon.vertices_begin(),
            polygon.vertices_end(),
            [ring](const Point_2 & vertex)
            {
                return ring->addPoint(to_ogr(vertex));
            }
        );
        ring->closeRings();
        return ring;
    }
    
    OGRPolygon* to_ogr(const Polygon_with_holes & polygon_with_holes)
    {
        OGRPolygon* ogr_polygon = new OGRPolygon();
        std::cout << "writing polygon" << std::endl;
        ogr_polygon->addRing(to_ogr(polygon_with_holes.outer_boundary()));
        std::for_each(
            polygon_with_holes.holes_begin(),
            polygon_with_holes.holes_end(),
            [ogr_polygon](const Polygon & hole)
            {
                ogr_polygon->addRing(to_ogr(hole));
            }
        );
        return ogr_polygon;
    }

    Point_2 to_urban(OGRPoint* ogr_point)
    {
        ExactToInexact to_exact;
        return urban::Point_2(to_exact(ogr_point->getX()), to_exact(ogr_point->getY()));
    }

    Polygon to_urban(OGRLinearRing* ogr_ring)
    {
        std::vector<Point_2> vertices(ogr_ring->getNumPoints() - 1);
        OGRPoint* ogr_vertex = NULL;
        for(int vertex_index(0); vertex_index < (ogr_ring->getNumPoints() - 1); ++vertex_index)
        {
            ogr_vertex = NULL;
            ogr_ring->getPoint(vertex_index, ogr_vertex);
            vertices[vertex_index] = to_urban(ogr_vertex);
        }
        std::free(ogr_vertex);
        return Polygon(std::begin(vertices), std::end(vertices));
    }
    
    Polygon_with_holes to_urban(OGRPolygon* ogr_polygon)
    {
        std::vector<Polygon> holes(ogr_polygon->getNumInteriorRings());
        for(int hole_index(0); hole_index < ogr_polygon->getNumInteriorRings(); ++hole_index)
            holes[hole_index] = to_urban(ogr_polygon->getInteriorRing(hole_index));
        return Polygon_with_holes(to_urban(ogr_polygon->getExteriorRing()));
    }

}
