#pragma once

#include "../geometry_definitions.h"
#include "../Projection/Face/face_projection.h"

#include <vector>
#include <map>

namespace urban
{
    /** Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation rotation_transform(const std::map<double, Vector> & _rotations);

    /** Check points collinearity*/
    bool check_collinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last);

    /** Get extrems from coliear points*/
    void extrem_points(std::vector<Point_2> & points);

    
    /**
     * Sorting heuristic functors
     */
    struct Heuristic
    {
        Heuristic(void);
        virtual ~Heuristic(void);
        virtual bool operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b) = 0;
    };

    struct SimpleHeuristic : public Heuristic
    {
        SimpleHeuristic(void);
        ~SimpleHeuristic(void);
        bool operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b);
    };

    // To be checked
    struct NaiveHeuristic : public Heuristic
    {
        NaiveHeuristic(void);
        ~NaiveHeuristic(void);
        bool operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b);
    };
}
