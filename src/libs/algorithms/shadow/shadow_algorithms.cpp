#include "shadow_algorithms.h"

#include "../../geometry_definitions.h"

#include <sstream>

#include <iterator>
#include <algorithm>

#include <stdexcept>

namespace urban
{
    shadow::Face & transform(shadow::Face & face, std::map<size_t, size_t> const& map)
    {
        std::vector<size_t> mapped_indexes(face.get_degree());
        std::transform(
            std::begin(face),
            std::end(face),
            std::begin(mapped_indexes),
            [&map](size_t const index)
            {
                return map.at(index);
            }
        );
        face = shadow::Face(mapped_indexes);
        return face;
    }


    std::map<size_t, size_t> & connectable(shadow::Mesh const& lhs, shadow::Mesh const& rhs, std::map<size_t, size_t> & suture_points)
    {
        if(!suture_points.empty())
            throw std::logic_error("The map must be empty");
        
        std::for_each(
            lhs.points_cbegin(),
            lhs.points_cend(),
            [&suture_points, &rhs](std::pair<size_t, shadow::Point> const& p)
            {
                auto suture_point = std::find_if(
                    rhs.points_cbegin(),
                    rhs.points_cend(),
                    [&p](std::pair<size_t, shadow::Point> const& q)
                    {
                        return p.second == q.second;
                    }
                );

                if(suture_point != rhs.points_cend())
                    suture_points.emplace(std::make_pair(suture_point->first, p.first));
            }
        );

        return suture_points;
    }

    shadow::Mesh stitch(shadow::Mesh const& lhs, shadow::Mesh const& rhs, std::map<size_t, size_t> const& suture_points)
    {
        std::map<size_t, shadow::Point> l_coordinates(lhs.get_points());
        
        std::map<size_t, shadow::Face>  l_faces(lhs.get_faces()),
                                        r_faces(rhs.get_faces());

        size_t shift(lhs.points_size());
        std::map<size_t, size_t> stitcher;
        std::for_each(
            rhs.points_cbegin(),
            rhs.points_cend(),
            [&l_coordinates, &shift, &suture_points, &stitcher](std::pair<size_t, shadow::Point> const& p)
            {
                auto suture_point = std::find_if(
                    std::begin(suture_points),
                    std::end(suture_points),
                    [&p](std::pair<size_t, size_t> const& map_r_l)
                    {
                        return map_r_l.first == p.first;
                    }
                );

                if(suture_point == std::end(suture_points))
                {
                    stitcher.emplace(std::make_pair(p.first, p.first + shift));
                    l_coordinates.emplace(std::make_pair(p.first + shift, p.second));
                }
                else
                {
                    stitcher.emplace(std::make_pair(p.first, suture_points.at(p.first)));
                    shift--;
                }
            }
        );

        shift = l_faces.size();
        std::for_each(
            std::begin(r_faces),
            std::end(r_faces),
            [&l_faces, &stitcher, shift](std::pair<const size_t, shadow::Face> p)
            {
                std::pair<size_t, shadow::Face> mapped_face(p.first + shift, transform(p.second, stitcher));
                auto found = std::find_if(
                    std::begin(l_faces),
                    std::end(l_faces),
                    [&mapped_face](std::pair<size_t, shadow::Face> const& q)
                    {
                        return q.second == mapped_face.second;
                    }
                );

                if(found == std::end(l_faces))
                    l_faces.emplace(mapped_face);
            }
        );

        return shadow::Mesh(
            lhs.get_name() + "_" + rhs.get_name(),
            l_coordinates,
            l_faces
        );
    }

    std::vector<shadow::Mesh> & stitch(std::vector<shadow::Mesh> & connex_meshes, shadow::Mesh & mesh)
    {
        std::vector<shadow::Mesh> local_buffer;
        local_buffer.reserve(connex_meshes.size() + 1);

        std::for_each(
            std::begin(connex_meshes),
            std::end(connex_meshes),
            [&mesh, &local_buffer](shadow::Mesh & connex_mesh)
            {
                std::map<size_t, size_t> suture_points;
                suture_points = connectable(connex_mesh, mesh, suture_points);
                if(!suture_points.empty())
                    mesh = stitch(connex_mesh, mesh, suture_points);
                else
                    local_buffer.push_back(connex_mesh);
            }
        );

        local_buffer.push_back(mesh);
        
        connex_meshes = std::move(local_buffer);
        return connex_meshes;
    }

    std::vector<shadow::Mesh> & stitch(std::vector<shadow::Mesh> & meshes)
    {
       auto buffer = std::accumulate(
           std::begin(meshes),
           std::end(meshes),
           std::vector<shadow::Mesh>(),
           [](std::vector<shadow::Mesh> & connex_meshes, shadow::Mesh & mesh)
           {
               return stitch(connex_meshes, mesh);
           }
       );
       meshes = std::move(buffer);
       return meshes;
    }
}
