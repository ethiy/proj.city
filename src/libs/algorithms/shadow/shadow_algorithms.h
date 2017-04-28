#pragma once

#include "../../shadow/mesh.h"

#include <map>
#include <vector>

namespace urban
{
    /**
     * Apply a map on a face
     * @param face face to change
     * @param map bijective map changing indexes on a face
     * @return mapped Face
     */
    shadow::Face & transform(shadow::Face & face, std::map<std::size_t, std::size_t> const& map);

    /**
     * Check if two meshes have common points
     * @param lhs first mesh to be stitched
     * @param rhs second mesh to be stitched
     * @param suture_points a map: index second mesh |--> index of common point in first mesh
     * @return suture points map
     */
    std::map<std::size_t, std::size_t> & connectable(shadow::Mesh const& lhs, shadow::Mesh const& rhs, std::map<std::size_t, std::size_t> & suture_points);
    
    /**
     * Stitch two meshes
     * @param lhs first mesh to be stitched
     * @param rhs second mesh to be stitched
     * @param suture_points a map index second mesh |--> index of common point in first mesh
     * @return lhs U rhs
     */
    shadow::Mesh stitch(shadow::Mesh const& lhs, shadow::Mesh const& rhs, std::map<std::size_t, std::size_t> const& suture_points);
    
    /**
     * Stitch a mesh to set of connex meshes
     * @param connex_meshes set of connex meshes
     * @param mesh mesh to be stitched
     * @return a connex vector of meshes
     */
    std::vector<shadow::Mesh> & stitch(std::vector<shadow::Mesh> & connex_meshes, shadow::Mesh & mesh);
    
    /**
     * Stitch a vector of meshes
     * @param meshes a vector of meshes to stitch
     * @return a vector of stitched connex meshes
     */
    std::vector<shadow::Mesh> & stitch(std::vector<shadow::Mesh> & meshes);
}
