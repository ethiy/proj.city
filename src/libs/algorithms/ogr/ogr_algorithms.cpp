#include "../ogr/ogr_algorithms.h"

#include <algorithm>
#include <vector>

namespace urban
{
    OGRPoint* to_ogr(const Point_2 & point, const shadow::Point & reference_point)
    {
        return new OGRPoint(to_double(point.x() + reference_point.x()), to_double(point.y() + reference_point.y()));
    }

    OGRLinearRing* to_ogr(const Polygon & polygon, const shadow::Point & reference_point)
    {
        OGRLinearRing* ring = new OGRLinearRing();
        std::for_each(
            polygon.vertices_begin(),
            polygon.vertices_end(),
            [ring, &reference_point](const Point_2 & vertex)
            {
                return ring->addPoint(to_ogr(vertex, reference_point));
            }
        );
        ring->closeRings();
        return ring;
    }
    
    OGRPolygon* to_ogr(const Polygon_with_holes & polygon_with_holes, const shadow::Point & reference_point)
    {
        OGRPolygon* ogr_polygon = new OGRPolygon();
        ogr_polygon->addRing(to_ogr(polygon_with_holes.outer_boundary(), reference_point));
        std::for_each(
            polygon_with_holes.holes_begin(),
            polygon_with_holes.holes_end(),
            [ogr_polygon, &reference_point](const Polygon & hole)
            {
                ogr_polygon->addRing(to_ogr(hole, reference_point));
            }
        );
        return ogr_polygon;
    }

    Point_2 get_ogr_point(OGRPoint* ogr_point)
    {
        InexactToExact to_exact;
        return urban::Point_2(to_exact(ogr_point->getX()), to_exact(ogr_point->getY()));
    }

    Polygon get_ogr_ring(OGRLinearRing* ogr_ring)
    {
        std::vector<Point_2> vertices(static_cast<size_t>(ogr_ring->getNumPoints()) - 1);
        std::vector<size_t> indexes(static_cast<size_t>(ogr_ring->getNumPoints()) - 1);
        std::iota(std::begin(indexes), std::end(indexes), 0);
        std::transform(
            std::begin(indexes),
            std::end(indexes),
            std::begin(vertices),
            [ogr_ring](size_t index)
            {
                OGRPoint* ogr_vertex = new OGRPoint();
                ogr_ring->getPoint(static_cast<int>(index), ogr_vertex);
                return get_ogr_point(ogr_vertex);
            }
        );
        return Polygon(std::begin(vertices), std::end(vertices));
    }
    
    Polygon_with_holes get_ogr_polygon(OGRPolygon* ogr_polygon)
    {
        std::vector<Polygon> holes(static_cast<size_t>(ogr_polygon->getNumInteriorRings()));
        std::vector<int> indexes(static_cast<size_t>(ogr_polygon->getNumInteriorRings()));
        std::iota(std::begin(indexes), std::end(indexes), 0);
        std::transform(
            std::begin(indexes),
            std::end(indexes),
            std::begin(holes),
            [ogr_polygon](int index)
            {
                return get_ogr_ring(ogr_polygon->getInteriorRing(index));
            }
        );
        return Polygon_with_holes(get_ogr_ring(ogr_polygon->getExteriorRing()), std::begin(holes), std::end(holes));
    }

}
