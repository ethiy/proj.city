#pragma once

#include <geometry_definitions.h>

#include <shadow/point.h>
#include <shadow/mesh.h>

#include <io/Adjacency_stream/adjacency_stream.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW

#include <string>
#include <ostream>

namespace city
{
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
                std::vector<Point_3> points,
                std::vector< std::vector<std::size_t> > polygons,
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
            Bbox_3 const& bbox(void) const noexcept;

            std::string const& get_name(void) const noexcept;
            /**
            * Access urban node epsg.
            * @return urban node epsg
            */
            unsigned short const& get_epsg(void) const noexcept;
            /**
            * Access urban node reference point.
            * @return urban node reference point
            */
            shadow::Point const& get_reference_point(void) const noexcept;
            /**
            * Access urban node surface.
            * @return urban node surface
            */
            Polyhedron const& get_surface(void) const noexcept;
            /**
            * Get the number of vertices
            * @return number of vertices
            */
            std::size_t vertices_size(void) const;
            /**
            * Get the number of facets
            * @return number of facets
            */
            std::size_t facets_size(void) const;

            using Halfedge = Polyhedron::Halfedge;
            /** Halfedge handle */
            using Halfedge_handle = Polyhedron::Halfedge_handle;
            /** Halfedge constant handle */
            using Halfedge_const_handle = Polyhedron::Halfedge_const_handle;
            /** Halfedge iterator*/
            using Halfedge_iterator = Polyhedron::Halfedge_iterator;
            /** Halfedge constant iterator*/
            using Halfedge_const_iterator = Polyhedron::Halfedge_const_iterator;

            /** Facet */
            using Facet = Polyhedron::Facet;
            /** Facet handle */
            using Facet_handle = Polyhedron::Facet_handle;
            /** Facet constant handle */
            using Facet_const_handle = Polyhedron::Facet_const_handle;
            /** Facet iterator */
            using Facet_iterator = Polyhedron::Facet_iterator;
            /** Facet constant iterator */
            using Facet_const_iterator = Polyhedron::Facet_const_iterator;

            /** Points iterator */
            using Point_iterator = Polyhedron::Point_iterator;
            /** Points constant iterator */
            using Point_const_iterator = Polyhedron::Point_const_iterator;

            /** Plane iterator */
            using Plane_iterator = Polyhedron::Plane_iterator;
            /** Plane constant iterator */
            using Plane_const_iterator = Polyhedron::Plane_const_iterator;

            /**
            * Returns facets begin iterator
            * @return facets begin iterator
            */
            Facet_iterator facets_begin(void) noexcept;
            /**
            * Returns facets end iterator
            * @return facets end iterator
            */
            Facet_iterator facets_end(void) noexcept;
            /**
            * Returns facets constant begin iterator
            * @return facets constant begin iterator
            */
            Facet_const_iterator facets_begin(void) const noexcept;
            /**
            * Returns facets constant end iterator
            * @return facets constant end iterator
            */
            Facet_const_iterator facets_end(void) const noexcept;
            /**
            * Returns facets constant begin iterator
            * @return facets constant begin iterator
            */
            Facet_const_iterator facets_cbegin(void) const noexcept;
            /**
            * Returns facets constant end iterator
            * @return facets constant end iterator
            */
            Facet_const_iterator facets_cend(void) const noexcept;

            /**
            * Returns halfedges begin iterator
            * @return halfedges begin iterator
            */
            Halfedge_iterator halfedges_begin(void) noexcept;
            /**
            * Returns halfedges end iterator
            * @return halfedges end iterator
            */
            Halfedge_iterator halfedges_end(void) noexcept;
            /**
            * Returns halfedges constant begin iterator
            * @return halfedges constant begin iterator
            */
            Halfedge_const_iterator halfedges_begin(void) const noexcept;
            /**
            * Returns halfedges constant end iterator
            * @return halfedges constant end iterator
            */
            Halfedge_const_iterator halfedges_end(void) const noexcept;
            /**
            * Returns halfedges constant begin iterator
            * @return halfedges constant begin iterator
            */
            Halfedge_const_iterator halfedges_cbegin(void) const noexcept;
            /**
            * Returns halfedges constant end iterator
            * @return halfedges constant end iterator
            */
            Halfedge_const_iterator halfedges_cend(void) const noexcept;
            /**
            * Returns border halfedges begin iterator
            * @return border halfedges begin iterator
            */
            Halfedge_iterator border_halfedges_begin(void) noexcept;
            /**
            * Returns border halfedges constant begin iterator
            * @return border halfedges constant begin iterator
            */
            Halfedge_const_iterator border_halfedges_begin(void) const noexcept;

            /**
            * Returns points begin iterator
            * @return points begin iterator
            */
            Point_iterator points_begin(void) noexcept;
            /**
            * Returns points end iterator
            * @return points end iterator
            */
            Point_iterator points_end(void) noexcept;
            /**
            * Returns points constant begin iterator
            * @return points constant begin iterator
            */
            Point_const_iterator points_cbegin(void) const noexcept;
            /**
            * Returns points constant end iterator
            * @return points constant end iterator
            */
            Point_const_iterator points_cend(void) const noexcept;

            /**
            * Returns planes begin iterator
            * @return planes begin iterator
            */
            Plane_iterator planes_begin(void) noexcept;
            /**
            * Returns planes end iterator
            * @return planes end iterator
            */
            Plane_iterator planes_end(void) noexcept;
            /**
            * Returns planes constant begin iterator
            * @return planes constant begin iterator
            */
            Plane_const_iterator planes_cbegin(void) const noexcept;
            /**
            * Returns planes constant end iterator
            * @return planes constant end iterator
            */
            Plane_const_iterator planes_cend(void) const noexcept;

            bool empty(void) const;

            /**
            * Compute the centroid of a urban node facet
            * @param facet a urban node facet
            * @return centroid of the facet
            */
            Point_3 centroid(UNode::Facet_const_handle facet) const;
            /**
            * Compute the normal of a urban node facet
            * @param facet a urban node facet
            * @return the normal of the facet
            */
            Vector_3 normal(UNode::Facet_const_handle facet) const;
            /**
            * Compute the area of a urban node facet
            * @param facet a urban node facet
            * @return area of the facet
            */
            double area(UNode::Facet_const_handle facet) const;
            /**
            * Compute the circumference of a urban node facet
            * @param facet a urban node facet
            * @return circumference of the facet
            */
            double circumference(UNode::Facet_const_handle facet) const;

            Plane_3 plane(UNode::Facet const& facet) const;
            
            UNode& set_face_ids(void);

            /** 
            * Finds a joinable halfedge.
            * @return a halfedge handle of joinable facets
            */
            UNode::Halfedge_iterator prunable(void);

            UNode& prune(void);

            /** 
            * Join prunable halfedge
            * Wraps CGAL join_facet() for urban node.
            * @param h halfedge handle to join its incidents facets
            * @return `this` urban node modified
            */
            UNode & join_facet(Halfedge_handle & h);

            UNode & stitch_borders(void);

            /** 
            * Get facets adjacent to a facet
            * @param facet a urban node facet
            * @return handle to facets adjacent to facet
            */
            std::vector<UNode::Facet_const_handle> facet_adjacents(UNode::Facet const& facet) const;
            std::vector<UNode::Facet_const_handle> facet_handles(void) const;
            /** 
            * Write facet adjacency matrix to building matrix
            * @return the adjacency matrix of facets
            */
            std::vector<bool> facet_adjacency_matrix(void) const;

            UNode& transform(const Affine_transformation_3 & affine_transformation);

            Polygon_with_holes facet_projection(UNode::Facet const& facet) const;
        private:
            /** Node name */
            std::string name;
            /** Reference Point */
            shadow::Point reference_point;
            /** Projection system EPSG code*/
            unsigned short epsg_index = 2154;
            /** The 3D surface*/
            Polyhedron surface;
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
            #ifdef CGAL_USE_GEOMVIEW
            /**
            * Outstreaming urban node to GeomView
            * @param gs the output stream
            * @param unode the unode to stream
            * @return the output stream
            */
            friend CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream & gs, UNode const& unode);
            #endif // CGAL_USE_GEOMVIEW
        };

        void swap(UNode & lhs, UNode & rhs);

        /*! Computes border length*/
        double border_length(UNode const& unode);

        /*! Applies affine transformations to unodes*/
        UNode & affine_transform(UNode & unode, const Affine_transformation_3 &);
        /*! Translate unodes*/
        UNode & translate(UNode & unode, const Vector_3 &);
        /*! Scale unodes*/
        UNode & scale(UNode & unode, double);
        /*! Rotate unodes*/
        UNode & rotate(UNode & unode, const Vector_3 &, double);
        /*! Rotate unodes*/
        UNode & rotate(UNode & unode, const std::map<double, Vector_3> &);

        /**
         * Prunes surfaces of unode.
         * @param unode the surface to prune
         * @return the surface with pruned facets
         */
        UNode & prune(UNode & unode);

        /**
         * Computes the unode surface area
         * @param unode surface of which the area is computed
         * @return the surface area
        */
        double area(UNode const& unode);

        /**
         * Computes the unode surface total edge length
         * @param unode surface of which the total edge length is computed
         * @return the unode surface total edge length
        */
        double total_edge_length(UNode const& unode);
    }
}
