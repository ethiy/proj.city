#include <scene/unode.h>

#include <projection/face_projection.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include <CGAL/boost/graph/Euler_operations.h>
#include <boost/range/iterator_range.hpp>

#include <vector>


namespace city
{
    namespace scene
    {
        UNode::UNode(void) 
        {}
        UNode::UNode(UNode const& other)
            :name(std::move(other.name)),
             reference_point(other.reference_point),
             epsg_index(other.epsg_index),
             surface(other.surface),
             bounding_box(other.bounding_box)
        {}
        UNode::UNode(UNode && other)
            :name(std::move(other.name)),
             reference_point(std::move(other.reference_point)),
             epsg_index(std::move(other.epsg_index)),
             surface(std::move(other.surface)),
             bounding_box(std::move(other.bounding_box))
        {}
        UNode::UNode(
            std::string const& node_id,
            std::vector<shadow::Mesh> const& meshes,
            shadow::Point const& _reference_point,
            unsigned short const _epsg_index
        )
            :name(node_id), reference_point(_reference_point), epsg_index(_epsg_index)
        {
            std::vector<Mesh> surface_meshes(meshes.size());
            std::transform(
                std::begin(meshes),
                std::end(meshes),
                std::begin(surface_meshes),
                [](shadow::Mesh const& mesh)
                {
                    std::vector<Point_3> points = mesh.get_cgal_points();
                    std::vector< std::vector<std::size_t> > polygons = mesh.get_cgal_faces();

                    Mesh surface_mesh;

                    CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
                    CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface_mesh);
                    CGAL::Polygon_mesh_processing::stitch_borders(surface_mesh);

                    return surface_mesh;
                }
            );

            surface = *std::begin(surface_meshes);

            for(auto & surface_mesh : boost::make_iterator_range(std::next(std::begin(surface_meshes)), std::end(surface_meshes)))
            {
                CGAL::Polygon_mesh_processing::corefine_and_compute_difference(surface, surface_mesh, surface);
                CGAL::Polygon_mesh_processing::corefine_and_compute_difference(surface_mesh, surface, surface_mesh);
                CGAL::Polygon_mesh_processing::corefine_and_compute_union(surface, surface_mesh, surface);
            }

            bounding_box = CGAL::Polygon_mesh_processing::bbox(surface);
        }
        UNode::UNode(
            shadow::Mesh const& mesh,
            shadow::Point const& _reference_point,
            unsigned short const _epsg_index
        )
            : name(mesh.get_name()), reference_point(_reference_point), epsg_index(_epsg_index)
        {
            std::vector<Point_3> points = mesh.get_cgal_points();
            std::vector< std::vector<std::size_t> > polygons = mesh.get_cgal_faces();

            CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface);
            if(CGAL::is_closed(surface) && !CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            bounding_box = CGAL::Polygon_mesh_processing::bbox(surface);
            CGAL::Polygon_mesh_processing::stitch_borders(surface);

            auto fccmap = surface.add_property_map<Mesh::Face_index, std::size_t>("f:CC").first;
            auto cc =  CGAL::Polygon_mesh_processing::connected_components(surface, fccmap);
            std::cout << name << " " << cc << std::endl;
        }
        UNode::UNode(
            std::string const& building_id,
            std::vector<Point_3> & points,
            std::vector< std::vector<std::size_t> > & polygons,
            shadow::Point const& _reference_point,
            unsigned short const _epsg_index
        )
            :name(building_id), reference_point(_reference_point), epsg_index(_epsg_index)
        {
            CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface);
            if (CGAL::is_closed(surface) && !CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            bounding_box = CGAL::Polygon_mesh_processing::bbox(surface);
        }
        UNode::~UNode(void)
        {}
        
        void UNode::swap(UNode & other)
        {
            using std::swap;

            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(epsg_index, other.epsg_index);
            swap(surface, other.surface);
            swap(bounding_box, other.bounding_box);
        }
        UNode & UNode::operator =(UNode const& other) noexcept
        {
            name = other.name;
            reference_point = other.reference_point;
            epsg_index = other.epsg_index;
            surface = other.surface;
            bounding_box = other.bounding_box;

            return *this;
        }
        UNode & UNode::operator =(UNode && other) noexcept
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            epsg_index = std::move(other.epsg_index);
            surface = std::move(other.surface);
            bounding_box = std::move(other.bounding_box);

            return *this;
        }

        Bbox_3 const& UNode::bbox(void) const noexcept
        {
            return bounding_box;
        }

        std::string const& UNode::get_name(void) const noexcept
        {
            return name;
        }

        unsigned short const& UNode::get_epsg(void) const noexcept
        {
            return epsg_index;
        }

        shadow::Point const& UNode::get_reference_point(void) const noexcept
        {
            return reference_point;
        }

        Mesh const& UNode::get_surface(void) const noexcept
        {
            return surface;
        }


        UNode::Halfedge_index UNode::prunable(void) const
        {
            return *std::find_if(
                std::begin(halfedges()),
                std::end(halfedges()),
                [this](Halfedge_index const& halfedge)
                {
                    return  !surface.is_border(surface.edge(halfedge))
                            &&
                            (
                                CGAL::cross_product(
                                    CGAL::Polygon_mesh_processing::compute_face_normal(surface.face(halfedge), surface),
                                    CGAL::Polygon_mesh_processing::compute_face_normal(surface.face(surface.opposite(halfedge)), surface)
                                )
                                ==
                                CGAL::NULL_VECTOR
                            );
                }
            );
        }

        std::vector<UNode::Halfedge_index> UNode::combinable(Face_index const& facet) const
        {
            std::vector<UNode::Halfedge_index> combining_edges;
            combining_edges.reserve(surface.degree(facet));
            for(auto const& halfedge : CGAL::halfedges_around_face(surface.halfedge(facet), surface))
            {
                if(!surface.is_border(surface.edge(halfedge)))
                {
                    Point_3 A(surface.point(surface.target(halfedge))),
                            B(surface.point(surface.target(surface.next(halfedge)))),
                            C(surface.point(surface.target(surface.next(surface.next(halfedge))))),
                            D(surface.point(surface.target(surface.next(surface.opposite(halfedge)))));
                    if(CGAL::determinant(B - A, C - A, D - A) == Kernel::FT(0))
                        combining_edges.push_back(halfedge);
                }
            }

            return combining_edges;
        }
        std::vector<UNode::Halfedge_index> UNode::pruning_halfedges(void)
        {
            std::vector<Halfedge_index> combining_edges;

            for(auto const& facet : faces())
            {
                auto buffer = combinable(facet);
                std::copy_if(
                    std::begin(buffer),
                    std::end(buffer),
                    std::back_inserter(combining_edges),
                    [this, &combining_edges](Halfedge_index const& h)
                    {
                        return std::none_of(
                            std::begin(combining_edges),
                            std::end(combining_edges),
                            [&h, this](Halfedge_index const& present)
                            {
                                return  (surface.point(surface.target(present)) == surface.point(surface.target(h)) && surface.point(surface.source(present)) == surface.point(surface.source(h)))
                                        ||
                                        (surface.point(surface.source(present)) == surface.point(surface.target(h)) && surface.point(surface.target(present)) == surface.point(surface.source(h)));
                            }
                        );
                    }
                );
            }

            return combining_edges;
        }
        UNode & UNode::join_facet(Halfedge_index & h)
        {
            CGAL::Euler::join_face(h, surface);
            surface.collect_garbage();
            return *this;
        }
        UNode & UNode::stitch_borders(void)
        {
            CGAL::Polygon_mesh_processing::stitch_borders(surface);
            if (CGAL::is_closed(surface) && !CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            
            return *this;
        }

        Point_3 UNode::centroid(Face_index const& facet) const
        {
            Vector_3 n = normal(facet);

            Vector_3 centroid = CGAL::NULL_VECTOR;
            for(auto const& halfedge : CGAL::halfedges_around_face(surface.halfedge(facet), surface))
                centroid =  centroid
                            +
                            ((surface.point(surface.target(halfedge)) - CGAL::ORIGIN) + (surface.point(surface.source(halfedge)) - CGAL::ORIGIN))
                                *
                            to_double(CGAL::cross_product(surface.point(surface.target(halfedge)) - CGAL::ORIGIN, surface.point(surface.source(halfedge)) - CGAL::ORIGIN) * n)
                                /
                            6;

            return CGAL::ORIGIN + centroid / area(facet);
        }
        Vector_3 UNode::normal(Face_index const& facet) const
        {
            return CGAL::Polygon_mesh_processing::compute_face_normal(facet, surface);
        }
        double UNode::area(Face_index const& facet) const
        {
            ExactToInexact to_inexact;
            return to_inexact(CGAL::Polygon_mesh_processing::face_area(facet, surface));
        }
        double UNode::circumference(Face_index const& facet) const
        {
            double result = 0;
            for(auto const& halfedge : CGAL::halfedges_around_face(surface.halfedge(facet), surface))
                result += std::sqrt(
                    to_double(
                        CGAL::squared_distance(
                            surface.point(surface.target(halfedge)),
                            surface.point(surface.source(halfedge))
                        )
                    )
                );

            return result;
        }

        Polygon_with_holes UNode::facet_projection(Face_index const& facet) const
        {
            std::vector<Point_2> facet_trace(surface.degree(facet));

            auto iter = std::begin(facet_trace);
            for(auto const vertex : CGAL::vertices_around_face(surface.halfedge(facet), surface))
                *iter++ = Point_2(surface.point(vertex).x(), surface.point(vertex).y());
            
            Polygon facet_proj(std::begin(facet_trace), std::end(facet_trace));

            if(facet_proj.is_simple() && facet_proj.orientation() == CGAL::CLOCKWISE)
                facet_proj.reverse_orientation();

            return Polygon_with_holes(facet_proj);
        }

        projection::FacePrint UNode::orthoprojection(Face_index const& facet) const
        {
            return projection::FacePrint(
                static_cast<std::size_t>(facet),
                facet_projection(facet),
                get_plane(facet)
            );
        }

        std::vector<projection::FacePrint> UNode::orthoprojections(void) const
        {
            std::vector<projection::FacePrint> prints(faces_size());

            std::transform(
                std::begin(faces()),
                std::end(faces()),
                std::begin(prints),
                [this](Face_index const& facet)
                {
                    return orthoprojection(facet);
                }
            );

            prints.erase(
                std::remove_if(
                    std::begin(prints),
                    std::end(prints),
                    [](projection::FacePrint const& facet)
                    {
                        return facet.is_empty() || facet.is_degenerate();
                    }
                ),
                std::end(prints)
            );

            return prints;
        }

        Plane_3 UNode::get_plane(Face_index const& facet) const
        {
            return Plane_3(
                surface.point(surface.source(surface.halfedge(facet))),
                normal(facet)
            );
        }

        std::vector<UNode::Face_index> UNode::facet_adjacents(Face_index const& facet) const
        {
            std::vector<Face_index> adjacents;
            adjacents.reserve(surface.degree(facet));

            for(auto const& halfedge : CGAL::halfedges_around_face(surface.halfedge(facet), surface))
                if(!surface.is_border(surface.edge(halfedge)))
                    adjacents.push_back(surface.face(surface.opposite(halfedge)));

            return adjacents;
        }
        std::vector<bool> UNode::facet_adjacency_matrix(void) const
        {
            std::vector<bool> matrix(faces_size() * faces_size(), false);

            for(std::size_t diag(0); diag != faces_size(); ++diag)
                matrix.at(diag * faces_size() + diag) = true;

            for(auto const& face : faces())
                for(auto const& adjacent : facet_adjacents(face))
                    matrix.at(
                        static_cast<std::size_t>(face) * faces_size()
                        +
                        static_cast<std::size_t>(adjacent)
                    ) = true;
            return matrix;
        }

        std::ostream & operator <<(std::ostream &os, UNode const& unode)
        {
            os  << "# Name: " << unode.name << std::endl
                << unode.surface;
            return os;
        }

        io::Adjacency_stream & operator <<(io::Adjacency_stream & as, UNode const& unode)
        {
            for(auto const& face: unode.faces())
                as << static_cast<std::size_t>(face) << " " << unode.surface.degree(face) << " " << unode.area(face) << " " << unode.circumference(face) << " " << unode.centroid(face) << " " << unode.normal(face) << std::endl;

            std::vector<bool> matrix = unode.facet_adjacency_matrix();

            as << matrix << std::endl;

            return as;
        }

        void swap(UNode & lhs, UNode & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
