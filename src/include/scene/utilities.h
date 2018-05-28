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
        
        Polygon local_transform(Affine_transformation_3 const& mapping, Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet, bool const invert = false);
        Polygon local_transform(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet);

        bool coplanar(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        bool opposite_coplanar(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        bool opposite_normals(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        std::vector<Polygon_with_holes> open_coplanar_intersection(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        bool do_open_coplanar_intersect(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);
        bool is_union_coplanar_simple(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs);

        Polyhedron polyhedron_from_polygon_soup(std::vector<Point_3> & points, std::vector< std::vector<std::size_t> > & polygons);
    }
}
