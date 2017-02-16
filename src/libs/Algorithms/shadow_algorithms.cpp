#include "shadow_algorithms.h"

#include "../geometry_definitions.h"

#include <sstream>

#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    bool connectable(const shadow::Mesh & lhs, const shadow::Mesh & rhs, std::map<size_t, size_t> & suture_points)
    {
        if(!suture_points.empty())
            throw std::logic_error("The map must be empty");
        
        std::map<size_t, Point> l_coordinates(lhs.get_points()),
                                r_coordinates(rhs.get_points());
        
        std::for_each(
            std::begin(l_coordinates),
            std::end(l_coordinates),
            [&r_coordinates, &suture_points](const std::pair<size_t, Point> & p)
            {
                auto suture_point = find_if(
                    std::begin(r_coordinates),
                    std::end(r_coordinates),
                    [&p](const std::pair<size_t, Point> & q)
                    {
                        return p.second == q.second;
                    }
                );

                if(suture_point != std::end(r_coordinates))
                    suture_points.emplace(std::make_pair(suture_point->first, p.first));
            }
        );

        return !suture_points.empty();
    }

    // shadow::Mesh stitch(const shadow::Mesh & lhs, const shadow::Mesh & rhs, const std::map<size_t, size_t> & suture_points)
    // {
    //     std::stringstream _name("");
    //     _name << lhs.get_name() << "_and_" << rhs.get_name();

    //     // std::for_each(

    //     // );
         

    //     // translate indexes s_coordinates

    //     // Find by value

    //     // concat coords

    //     // update s_faces indexes

    //     // concat faces
        
    //     shadow::Mesh stitched;
    //     return stitched;
    // }

    std::vector<shadow::Mesh> stitch(const std::vector<shadow::Mesh> & connex_meshes, const shadow::Mesh & mesh)
    {
        std::vector<shadow::Mesh> result;
        result.reserve(connex_meshes.size() + 1);
        
        shadow::Mesh snowball(mesh);

        std::for_each(
            std::begin(connex_meshes),
            std::end(connex_meshes),
            [&snowball, &result](const shadow::Mesh & connex_mesh)
            {
                std::map<size_t,size_t> suture_points;
                if(connectable(connex_mesh, snowball, suture_points))
                    snowball = stitch(connex_mesh, snowball, suture_points);
                else
                    result.push_back(connex_mesh);
            }
        );

        result.push_back(snowball);
        return result;
    }

    std::vector<shadow::Mesh> stitch(const std::vector<shadow::Mesh> & meshes)
    {
        if(meshes.empty())
            throw std::logic_error("Nothing to stitch here!");

       return std::accumulate(
           std::begin(meshes),
           std::end(meshes),
           std::vector<shadow::Mesh>(),
           [](const std::vector<shadow::Mesh> & result, const shadow::Mesh & mesh)
           {
               return stitch(result, mesh);
           }
       );
    }
}
