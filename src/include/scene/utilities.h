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
        Affine_transformation_3 reference_transform(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet);
        
        Polygon local_transform(Affine_transformation_3 const& mapping, Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet);
        Polygon local_transform(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet);

        bool coplanar(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        bool opposite_coplanar(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        bool open_coplanar_intersection(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
    }
}
