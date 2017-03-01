#pragma once

#include "../geometry_definitions.h"
#include "../Projection/Face/face_projection.h"

#include <ogr_geometry.h>

#include <vector>
#include <map>

namespace urban
{
    /** Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation_3 rotation_transform(const std::map<double, Vector_3> & _rotations);

    /** Check points collinearity*/
    bool check_collinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last);

    /** Get extrems from coliear points*/
    void extrem_points(std::vector<Point_2> & points);

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

    
    /**
     * Sorting heuristic functors
     */
    struct Heuristic
    {
        Heuristic(void);
        Heuristic(const Heuristic & other);
        Heuristic(Heuristic && other);
        virtual ~Heuristic(void);
        virtual bool operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b) = 0;
    };

    struct SimpleHeuristic : public Heuristic
    {
        SimpleHeuristic(void);
        SimpleHeuristic(const SimpleHeuristic & other);
        SimpleHeuristic(SimpleHeuristic && other);
        ~SimpleHeuristic(void);
        bool operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b);
    };

    // To be checked
    struct NaiveHeuristic : public Heuristic
    {
        NaiveHeuristic(void);
        NaiveHeuristic(const NaiveHeuristic & other);
        NaiveHeuristic(NaiveHeuristic && other);
        ~NaiveHeuristic(void);
        bool operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b);
    };
}
