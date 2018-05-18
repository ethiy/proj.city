# pragma once

#include <geometry_definitions.h>

#include <array>

namespace city
{
    namespace scene
    {
        bool is_orthonormal_direct(std::array<Vector_3, 3> const& reference_basis);

        Affine_transformation_3 reference_transform(std::array<Vector_3, 3> const& reference_basis);
        Affine_transformation_3 reference_transform(Point_3 const& source, Point_3 const& target, Vector_3 const& k);
        Affine_transformation_3 reference_transform(Mesh const& mesh, Mesh::Face_index const& facet);
        
        Polygon local_transform(Affine_transformation_3 const& mapping, Mesh const& mesh, Mesh::Face_index const& facet);
        Polygon local_transform(Mesh const& mesh, Mesh::Face_index const& facet);

        bool coplanar(Mesh const& mesh, Mesh::Face_index const& lhs, Mesh::Face_index const& rhs);
        bool opposite_coplanar(Mesh const& mesh, Mesh::Face_index const& lhs, Mesh::Face_index const& rhs);
        bool open_coplanar_intersection(Mesh const& mesh, Mesh::Face_index const& lhs, Mesh::Face_index const& rhs);
    }
}
