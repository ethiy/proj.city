#pragma once

#include "../geometry_definitions.h"
#include "../UrbanObject/brick.h"
#include "../Projection/face_projection.h"
#include "../Projection/brick_projection.h"
#include "../Projection/camera.h"

#include <vector>
#include <list>

namespace urban
{
    /*________________________ Utilities _________________________________________________*/

    /** Get the rotation transformation from a set of axis and angle couples*/
    Affine_transformation rotation_transform(const std::map<double, Vector> & _rotations);

    /** Check points collinearity*/
    bool check_collinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last);

    /** Get extrems from coliear points*/
    void extrem_points(std::vector<Point_2> & points);

    /** Compute FaceProjection area*/
    double area(const FaceProjection & facet);

    /**
     * Sorting heuristic functors
     */
    struct Heuristic
    {
        inline Heuristic(void){}
        virtual bool operator()(const FaceProjection & facet_a, const FaceProjection & facet_b) = 0;
    };

    struct SimpleHeuristic : public Heuristic
    {
        inline SimpleHeuristic(void){}
        inline bool operator()(const FaceProjection & facet_a, const FaceProjection & facet_b)
        {
            /* If one of the faces is perpendicular do not bother changing order
             */
            bool greater(false); 
            if(!facet_a.is_perpendicular() && !facet_b.is_perpendicular())
            {
                Point_2 point_a(
                    CGAL::centroid(
                        facet_a.outer_boundary()[0],
                        facet_a.outer_boundary()[1],
                        facet_a.outer_boundary()[2]
                    )
                );
                Point_2 point_b(
                    CGAL::centroid(
                        facet_b.outer_boundary()[0],
                        facet_b.outer_boundary()[1],
                        facet_b.outer_boundary()[2]
                    )
                );
                greater = facet_b.get_plane_height(point_b) < facet_a.get_plane_height(point_a);
            }
            return greater;
        }
    };

    // To be checked
    struct NaiveHeuristic : public Heuristic
    {
        inline NaiveHeuristic(void){}
        inline bool operator()(const FaceProjection & facet_a, const FaceProjection & facet_b)
        {
            bool greater(false); 
            if(!facet_a.is_perpendicular() && !facet_b.is_perpendicular())
            {
                auto m_a = std::min_element(
                    facet_a.outer_boundary().vertices_begin(),
                    facet_a.outer_boundary().vertices_end(),
                    [& facet_a](const Point_2 & A, const Point_2 & B)
                    {
                        return facet_a.get_plane_height(A) < facet_a.get_plane_height(B);
                    }
                );
                auto m_b = std::max_element(
                    facet_b.outer_boundary().vertices_begin(),
                    facet_b.outer_boundary().vertices_end(),
                    [& facet_b](const Point_2 & A, const Point_2 & B)
                    {
                        return facet_b.get_plane_height(A) < facet_b.get_plane_height(B);
                    }
                );
                greater = facet_b.get_plane_height(*m_b) < facet_a.get_plane_height(*m_a);
            }
            return greater;
        }
    };


    /*________________________ Projections _________________________________________________*/

    /** Projects on camera view*/
    BrickProjection project(const Brick & brick, const projection::Camera & camera);

    /* ! Projects on xy axis*/
    BrickProjection project(const Brick & brick);
    
    /** Project Faces to XY*/
    std::vector<FaceProjection> project_xy(const Brick & brick);

    /** Computes FaceProjection occlutions and gets rid of all perpendicular facets*/
    std::list<FaceProjection> occlusion(const FaceProjection & lhs, std::list<FaceProjection> & rhs);
}
