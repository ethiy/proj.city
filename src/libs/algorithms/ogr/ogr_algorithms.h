#pragma once

#include "../../geometry_definitions.h"
#include "../../shadow/Geometry/point.h"

#include <ogr_geometry.h>

namespace urban
{
    /**
     * construct OGRPoint from a CGAL Point_2
     * @param point a CGAL Point_2
     * @return a pointer to an OGR Point
     */
    OGRPoint* to_ogr(const Point_2 & point, const shadow::Point & reference_point);

    /**
     * construct OGRPolygon from a CGAL Polygon
     * @param polygon a CGAL Polygon
     * @return a pointer to an OGR Linear Ring
     */
    OGRLinearRing* to_ogr(const Polygon & polygon, const shadow::Point & reference_point);
    
    /**
     * construct OGRPolygon from a CGAL Polygon with holes
     * @param polygon_with_holes a CGAL Polygon with holes
     * @return a pointer to an OGR polygon
     */
    OGRPolygon* to_ogr(const Polygon_with_holes & polygon_with_holes, const shadow::Point & reference_point);
    
    /**
     * construct Point_2 from a OGRPoint
     * @param ogr_point a pointer to an OGRPoint
     * @return a Point_2
     */
    Point_2 get_ogr_point(OGRPoint* ogr_point);

    /**
     * construct Polygon from a OGRLinearRing
     * @param ogr_ring a pointer to an OGRLinearRing
     * @return a Polygon
     */
    Polygon get_ogr_ring(OGRLinearRing* ogr_ring);
    
    /**
     * construct Polygon with holes from a OGRPolygon
     * @param ogr_polygon a pointer to an OGRPolygon
     * @return a Polygon_with_holes
     */
    Polygon_with_holes get_ogr_polygon(OGRPolygon* ogr_polygon);
}
