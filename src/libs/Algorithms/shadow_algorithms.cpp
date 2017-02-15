#include "shadow_algorithms.h"

#include "../geometry_definitions.h"

#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>

namespace urban
{
    bool stitch(const shadow::Mesh & lhs, const shadow::Mesh & rhs, shadow::Mesh & stitched)
    {
        std::stringstream _name("");
        _name << lhs.get_name() << "_and_" << rhs.get_name();

        std::map<size_t, Point> s_coordinates, b_coordinates;
        std::map<size_t, shadow::Face> s_faces, b_faces;
        /**
         * This is an optimization
         *  - We are doing searches on the biggest mesh.
         */
        if(lhs.get_number_points() <= rhs.get_number_points())
        {
            s_coordinates = lhs.get_points();
            s_faces = lhs.get_faces();
            b_coordinates = rhs.get_points();
            b_faces = rhs.get_faces();
        }
        else
        {
            s_coordinates = rhs.get_points();
            s_faces = rhs.get_faces();
            b_coordinates = lhs.get_points();
            b_faces = lhs.get_faces();
        }

        size_t shift(s_coordinates.size());
        
        /**
         * Find common points;
         *  - Complexity: O( shift x log(b_coordinates.size()) )
         */
        std::map<size_t, size_t> suture_points;
        std::for_each(
            std::begin(s_coordinates),
            std::end(s_coordinates),
            [sihft](const std::pair<size_t, Point> & p)
            {
                auto suture_point = std::find_if(
                    std::begin(b_coordinates),
                    std::end(b_coordinates),
                    [&p](const std::pair<size_t, Point> & q)
                    {
                        return q.second == p.second;
                    }
                );

                if(suture_point != std::end(b_coordinates))
                    suture_points.emplace(std::make_pair(p.first, suture_point->first));
                else
                    suture_points.emplace(std::make_pair(p.first, p.first + shift))
            }
        );

        std::for_each(
            
        );
         

        // translate indexes s_coordinates

        // Find by value

        // concat coords

        // update s_faces indexes

        // concat faces
        
        stitched = std::move(shadow::Mesh());

        return false;
    }
}
