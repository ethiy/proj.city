#pragma once

#include <geometry_definitions.h>

#include <shadow/point.h>
#include <shadow/mesh.h>

#include <scene/utilities.h>

#include <io/Adjacency_stream/adjacency_stream.h>

#include <boost/property_map/property_map.hpp>

#include <string>
#include <ostream>

namespace city
{
    namespace projection
    {
        class FacePrint;
    }

    namespace scene
    {
        class UNode
        {
        public:
            UNode(void);
            UNode(UNode const& other);
            UNode(UNode && other);
            UNode(
                shadow::Mesh const& mesh,
                shadow::Point const& _reference_point=shadow::Point(),
                unsigned short const _epsg_index=2154
            );
            UNode(
                std::string const& node_id,
                std::vector<shadow::Mesh> const& meshes,
                shadow::Point const& _reference_point=shadow::Point(),
                unsigned short const _epsg_index=2154
            );
            UNode(
                std::string const& node_id,
                std::vector<Point_3> & points,
                std::vector< std::vector<std::size_t> > & polygons,
                shadow::Point const& _reference_point=shadow::Point(),
                unsigned short const _epsg_index=2154
            );
            ~UNode(void);

            void swap(UNode & other);
            UNode & operator =(UNode const& other) noexcept;
            UNode & operator =(UNode && other) noexcept;

            /** 
             * Computes the Building bounding box.
             * @return this bounding box
             */
            inline Bbox_3 const& bbox(void) const noexcept
            {
                return bounding_box;
            }

            inline std::string const& get_name(void) const noexcept
            {
                return name;
            }
            /**
            * Access urban node epsg.
            * @return urban node epsg
            */
            inline unsigned short const& get_epsg(void) const noexcept
            {
                return epsg_index;
            }
            /**
            * Access urban node reference point.
            * @return urban node reference point
            */
            inline shadow::Point const& get_reference_point(void) const noexcept
            {
                return reference_point;
            }
            /**
            * Access urban node surface.
            * @return urban node surface
            */
            inline Mesh const& get_surface(void) const noexcept
            {
                return surface;
            }
            /**
            * Get the number of vertices
            * @return number of vertices
            */
            inline std::size_t vertices_size(void) const
            {
                return surface.number_of_vertices();
            }
            /**
            * Get the number of facets
            * @return number of facets
            */
            inline std::size_t faces_size(void) const
            {
                return surface.number_of_faces();
            }

            using Halfedge_index = Mesh::Halfedge_index;
            using Halfedge_range = Mesh::Halfedge_range;

            using Face_index = Mesh::Face_index;
            using Face_range = Mesh::Face_range;

            using Vertex_index = Mesh::Vertex_index;
            using Vertex_range = Mesh::Vertex_range;

            using Edge_index = Mesh::Edge_index;
            using Edge_range = Mesh::Edge_range;

            inline Face_range faces(void) noexcept
            {
                return surface.faces();
            }
            inline Face_range faces(void) const noexcept
            {
                return surface.faces();
            }
            inline Halfedge_range halfedges(void) noexcept
            {
                return surface.halfedges();
            }
            inline Halfedge_range halfedges(void) const noexcept
            {
                return surface.halfedges();
            }
            inline Vertex_range vertices(void) noexcept
            {
                return surface.vertices();
            }
            inline Edge_range edges(void) noexcept
            {
                return surface.edges();
            }

            inline Halfedge_index null_halfedge() const noexcept
            {
                return surface.null_halfedge();
            }
            inline Face_index null_face() const noexcept
            {
                return surface.null_face();
            }
            inline Edge_index null_edge() const noexcept
            {
                return surface.null_edge();
            }
            inline Vertex_index null_vertex() const noexcept
            {
                return surface.null_vertex();
            }

            inline bool is_border(Halfedge_index const& halfedge) const
            {
                return surface.is_border(halfedge);
            }
            inline Vertex_index target(Halfedge_index const& halfedge) const
            {
                return surface.target(halfedge);
            }
            inline Vertex_index source(Halfedge_index const& halfedge) const
            {
                return surface.source(halfedge);
            }

            inline Point_3 point(Vertex_index const& vertex) const
            {
                return surface.point(vertex);
            }
            inline Mesh::Property_map<Vertex_index, Point_3> points(void) const
            {
                return surface.points();
            }
            inline Mesh::Property_map<Vertex_index, Point_3> & points(void)
            {
                return surface.points();
            }

            /**
            * Compute the centroid of a urban node facet
            * @param facet a urban node facet
            * @return centroid of the facet
            */
            Point_3 centroid(Face_index const& facet) const;
            /**
            * Compute the normal of a urban node facet
            * @param facet a urban node facet
            * @return the normal of the facet
            */
            Vector_3 normal(Face_index const& facet) const;
            /**
            * Compute the area of a urban node facet
            * @param facet a urban node facet
            * @return area of the facet
            */
            double area(Face_index const& facet) const;
            /**
            * Compute the circumference of a urban node facet
            * @param facet a urban node facet
            * @return circumference of the facet
            */
            double circumference(Face_index const& facet) const;

            Halfedge_index prunable(void) const;
            /** 
            * Join prunable halfedge
            * Wraps CGAL join_facet() for urban node.
            * @param h halfedge handle to join its incidents facets
            * @return `this` urban node modified
            */
            UNode & join_facet(Halfedge_index & h);

            UNode & stitch_borders(void);

            /** 
            * Get facets adjacent to a facet
            * @param facet a urban node facet
            * @return handle to facets adjacent to facet
            */
            std::vector<Face_index> facet_adjacents(Face_index const& facet) const;
            /** 
            * Write facet adjacency matrix to building matrix
            * @return the adjacency matrix of facets
            */
            std::vector<bool> facet_adjacency_matrix(void) const;

            Polygon_with_holes facet_projection(Face_index const& facet) const;
            projection::FacePrint orthoprojection(Face_index const& facet) const;
            std::vector<projection::FacePrint> orthoprojections(void) const;

            Plane_3 get_plane(Face_index const& facet) const;
        private:
            /** Node name */
            std::string name;
            /** Reference Point */
            shadow::Point reference_point;
            /** Projection system EPSG code*/
            unsigned short epsg_index = 2154;
            /** The 3D surface*/
            Mesh surface;
            /** Bounding box*/
            Bbox_3 bounding_box;

            /**
            * Outstreaming urban node in OFF format
            * @param os the output stream
            * @param unode the brick to stream
            * @return the output stream
            */
            friend std::ostream& operator<<(std::ostream & os, UNode const& unode);
            /**
            * Outstreaming the dual adjacency graph
            * @param as the output stream
            * @param unode the unode to stream
            * @return the output stream
            */
            friend io::Adjacency_stream& operator<<(io::Adjacency_stream & as, UNode const& unode);

            struct EdgeConstraint: public boost::put_get_helper< bool, EdgeConstraint>
            {
                EdgeConstraint(Mesh & _mesh)
                    : mesh(_mesh)
                {}

                bool operator[](Edge_index const& edge) const
                {
                    auto h = mesh.halfedge(edge);
                    return  !mesh.is_border(edge)
                            &&
                            coplanar(mesh, mesh.face(h), mesh.face(mesh.opposite(h)))
                            &&
                            !open_coplanar_intersection(mesh, mesh.face(h), mesh.face(mesh.opposite(h)));
                }
                
                Mesh & mesh;
            };
        };

        void swap(UNode & lhs, UNode & rhs);
    }
}
