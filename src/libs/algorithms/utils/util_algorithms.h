#pragma once

#include "../../geometry_definitions.h"
#include "../../projection/Face/face_projection.h"

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

    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(const InexactPolygon & polygon);
    
    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(const InexactPolygon_with_holes & polygon);

    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(const Polygon & polygon);
    
    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(const Polygon_with_holes & polygon);
    
    /**
     * Sorting heuristic functors
     */
    struct Heuristic
    {
        Heuristic(void) {}
        Heuristic(const Heuristic & ) {}
        Heuristic(Heuristic && ) {}
        virtual ~Heuristic(void) {}
        virtual bool operator()(projection::FacePrint const& facet_a, projection::FacePrint const& facet_b) = 0;
    };

    struct SimpleHeuristic : public Heuristic
    {
        SimpleHeuristic(void) {}
        SimpleHeuristic(SimpleHeuristic const& other): Heuristic(other) {}
        SimpleHeuristic(SimpleHeuristic && ) {}
        ~SimpleHeuristic(void) {}
        bool operator()(projection::FacePrint const& facet_a, projection::FacePrint const& facet_b);
    };

    // To be checked
    struct NaiveHeuristic : public Heuristic
    {
        NaiveHeuristic(void) {}
        NaiveHeuristic(NaiveHeuristic const& other): Heuristic(other) {}
        NaiveHeuristic(NaiveHeuristic && ) {}
        ~NaiveHeuristic(void) {}
        bool operator()(projection::FacePrint const& facet_a, projection::FacePrint const& facet_b);
    };
}
