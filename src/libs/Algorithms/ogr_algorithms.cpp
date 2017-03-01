#include "ogr_algorithms.h"

#include <algorithm>

namespace urban
{
    OGRPoint* to_ogr(const Point_2 & point)
    {
        OGRPoint* ogr_point = new OGRPoint();
        ogr_point->setX(to_double(point.x()));
        ogr_point->setY(to_double(point.y()));
        return ogr_point;
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
}
