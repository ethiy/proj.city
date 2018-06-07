#include <scene/utilities.h>

#include <algorithms/util_algorithms.h>

#include <CGAL/number_utils.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>

#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/boost/graph/iterator.h>

#include <iterator>
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
            if(!CGAL::is_zero(k * i))
                throw std::runtime_error("the normal must be orthogonal to the line formed by both points!");
            return reference_transform(
                std::array<Vector_3, 3>{{
                    i,
                    CGAL::cross_product(k, i),
                    k
                }}
            ) * Affine_transformation_3(CGAL::TRANSLATION, CGAL::ORIGIN - source);
        }
        Affine_transformation_3 reference_transform(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet)
        {
            return reference_transform(
                facet->halfedge()->opposite()->vertex()->point(),
                facet->halfedge()->vertex()->point(),
                CGAL::Polygon_mesh_processing::compute_face_normal(facet, polyhedron)
            );
        }

        Polygon local_transform(Affine_transformation_3 const& mapping, Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet, bool const invert)
        {
            std::vector<Point_2> transformed_points(facet->facet_degree());
            auto inserter = std::begin(transformed_points);
            for(auto const vertex : CGAL::vertices_around_face(facet->halfedge(), polyhedron))
            {
                auto p = mapping.transform(vertex->point());
                *inserter++ = Point_2(p.x(), p.y());
            }
            return invert? Polygon(
                transformed_points.rbegin(),
                transformed_points.rend()
            ) : Polygon(
                std::begin(transformed_points),
                std::end(transformed_points)
            );
        }
        Polygon local_transform(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& facet)
        {
            return local_transform(
                reference_transform(polyhedron, facet),
                polyhedron,
                facet
            );
        }


        bool coplanar(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs)
        {
            return CGAL::is_zero(
                CGAL::cross_product(
                    CGAL::Polygon_mesh_processing::compute_face_normal(lhs, polyhedron),
                    CGAL::Polygon_mesh_processing::compute_face_normal(rhs, polyhedron)
                ).squared_length()
            );
        }
        bool opposite_coplanar(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs)
        {
            return  coplanar(polyhedron, lhs, rhs)
                    &&
                    opposite_normals(polyhedron, lhs, rhs);
        }
        bool opposite_normals(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs)
        {
            return CGAL::is_negative(
                CGAL::Polygon_mesh_processing::compute_face_normal(lhs, polyhedron)
                *
                CGAL::Polygon_mesh_processing::compute_face_normal(rhs, polyhedron)
            );
        }
        std::vector<Polygon_with_holes> open_coplanar_intersection(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs)
        {
            if(!coplanar(polyhedron, lhs, rhs))
                throw std::runtime_error("Both faces are not coplanar!");

            std::vector<Polygon_with_holes> intersections;
            intersections.reserve(1);
            auto local_transformer = reference_transform(polyhedron, lhs);
            CGAL::intersection(
                local_transform(
                    local_transformer,
                    polyhedron,
                    lhs
                ),
                local_transform(
                    local_transformer,
                    polyhedron,
                    rhs,
                    opposite_normals(polyhedron, lhs, rhs)
                ),
                std::back_inserter(intersections)
            );
            return intersections;
        }
        bool do_open_coplanar_intersect(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs)
        {
            auto intersections = open_coplanar_intersection(polyhedron, lhs, rhs);
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
        bool is_union_coplanar_simple(Polyhedron const& polyhedron, Polyhedron::Facet_handle const& lhs, Polyhedron::Facet_handle const& rhs)
        {
            Polygon_with_holes joint_facet;
            auto local_transformer = reference_transform(polyhedron, lhs);            
            CGAL::join(
                local_transform(
                    local_transformer,
                    polyhedron,
                    lhs
                ),
                local_transform(
                    local_transformer,
                    polyhedron,
                    rhs,
                    opposite_normals(polyhedron, lhs, rhs)
                ),
                joint_facet
            );
            return joint_facet.outer_boundary().is_simple();
        }

        Polyhedron polyhedron_from_polygon_soup(std::vector<Point_3> & points, std::vector< std::vector<std::size_t> > & polygons)
        {
            Polyhedron surface;
            if(!points.empty() && !polygons.empty())
            {
            CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface);
            if(CGAL::is_closed(surface) && !CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            }
            return surface;
        }
    }
}
