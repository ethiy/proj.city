#include <scene/utilities.h>

#include <algorithms/util_algorithms.h>

#include <CGAL/number_utils.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/boost/graph/iterator.h>

#include <stdexcept>

namespace city
{
    namespace scene
    {
        bool is_orthonormal_direct(std::array<Vector_3, 3> const& reference_system)
        {
            return std::all_of(
                std::begin(reference_system),
                std::end(reference_system),
                [](Vector_3 const& v)
                {
                    return CGAL::is_one(v.squared_length());
                }
            )
            &&
            (
                CGAL::cross_product(reference_system.at(0), reference_system.at(1)) == reference_system.at(2)
            );
        }
        Affine_transformation_3 reference_transform(std::array<Vector_3, 3> const& reference_system)
        {
            if(is_orthonormal_direct(reference_system))
                throw std::runtime_error("The new reference system is not orthonormal and direct!");

            return Affine_transformation_3(
                reference_system.at(0).x(),
                reference_system.at(0).y(),
                reference_system.at(0).z(),
                reference_system.at(1).x(),
                reference_system.at(1).y(),
                reference_system.at(1).z(),
                reference_system.at(2).x(),
                reference_system.at(2).y(),
                reference_system.at(2).z()
            );
        }
        Affine_transformation_3 reference_transform(Point_3 const& source, Point_3 const& target, Vector_3 const& k)
        {
            auto i = target - source;
            i = i / std::sqrt(to_double(i.squared_length()));
            if(CGAL::is_zero(k * i))
                throw std::runtime_error("the normal must be orthogonal to the line formed by both points!");

            return reference_transform(
                std::array<Vector_3, 3>{{
                    i,
                    CGAL::cross_product(k, i),
                    k
                }}
            ) * Affine_transformation_3(CGAL::TRANSLATION, source - CGAL::ORIGIN);
        }
        Affine_transformation_3 reference_transform(Mesh const& mesh, Mesh::Face_index const& facet)
        {
            return reference_transform(
                mesh.point(mesh.source(mesh.halfedge(facet))),
                mesh.point(mesh.target(mesh.halfedge(facet))),
                CGAL::Polygon_mesh_processing::compute_face_normal(facet, mesh)
            );
        }

        Polygon local_transform(Affine_transformation_3 const& mapping, Mesh const& mesh, Mesh::Face_index const& facet)
        {
            std::vector<Point_2> transformed_points(mesh.degree(facet));
            auto inserter = std::begin(transformed_points);
            for(auto const vertex : CGAL::vertices_around_face(mesh.halfedge(facet), mesh))
            {
                auto p = mapping.transform(mesh.point(vertex));
                std::cout << p << std::endl;
                *inserter++ = Point_2(p.x(), p.y());
            }
            return Polygon(
                std::begin(transformed_points),
                std::end(transformed_points)
            );
        }

        bool coplanar(Mesh const& mesh, Mesh::Face_index const& lhs, Mesh::Face_index const& rhs)
        {
            return CGAL::cross_product(
                CGAL::Polygon_mesh_processing::compute_face_normal(lhs, mesh),
                CGAL::Polygon_mesh_processing::compute_face_normal(rhs, mesh)
            ) == CGAL::NULL_VECTOR;
        }
        bool open_coplanar_intersection(Mesh const& mesh, Mesh::Face_index const& lhs, Mesh::Face_index const& rhs)
        {
            if(!coplanar(mesh, lhs, rhs))
                throw std::runtime_error("Both faces are not coplanar!");

            auto tr = reference_transform(mesh, lhs);
            std::vector<Polygon_with_holes> intersections;
            intersections.reserve(1);
            CGAL::intersection(
                local_transform(tr, mesh, lhs),
                local_transform(tr, mesh, rhs),
                std::back_inserter(intersections)
            );
            return !CGAL::is_zero(
                std::accumulate(
                    std::begin(intersections),
                    std::end(intersections),
                    double(0),
                    [](double const area, Polygon_with_holes const& p)
                    {
                        return area + ::city::area(p);
                    }
                )
            );
        }
    }
}
