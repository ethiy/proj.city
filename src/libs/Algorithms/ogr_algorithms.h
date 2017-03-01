#pragma once

#include "../geometry_definitions.h"

#include <ogr_geometry.h>

namespace urban
{
    /**
     * construct OGRPoint from a CGAL Point_2
     * @param point a CGAL Point_2
     * @return a pointer to an OGR Point
     */
    OGRPoint* to_ogr(const Point_2 & point);

    /**
     * construct OGRPolygon from a CGAL Polygon
     * @param polygon a CGAL Polygon
     * @return a pointer to an OGR Linear Ring
     */
    OGRLinearRing* to_ogr(const Polygon & polygon);
    
    /**
     * construct OGRPolygon from a CGAL Polygon with holes
     * @param polygon a CGAL Polygon with holes
     * @return a pointer to an OGR polygon
     */
    OGRPolygon* to_ogr(const Polygon_with_holes & polygon_with_holes);
}
