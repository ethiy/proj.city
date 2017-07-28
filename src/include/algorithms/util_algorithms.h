#pragma once

#include "../geometry_definitions.h"
#include "../../shadow/mesh.h"
#include "../../projection/face/face_projection.h"

#include <ogr_geometry.h>

#include <vector>
#include <utility>
#include <map>

namespace urban
{
    /** Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation_3 rotation_transform(std::map<double, Vector_3> const& _rotations);

    /** Check points collinearity*/
    bool check_collinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last);

    /** Get extrems from colinear points*/
    std::pair<Point_2, Point_2> extrem_points(std::vector<Point_2> & points);


    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(InexactPolygon const& polygon);
    
    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(InexactPolygon_with_holes const& polygon);

    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(Polygon const& polygon);
    
    /** Get an approximate centroid
     * @param polygon an inexact polygon
     * @return an inexact point centroid
     */
    InexactPoint_2 centroid(Polygon_with_holes const& polygon);
    
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
