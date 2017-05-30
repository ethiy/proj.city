#pragma once

#include "../../geometry_definitions.h"

#include "../../shadow/mesh.h"

#include "../../io/Adjacency_stream/adjacency_stream.h"

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW

#include <string>
#include <ostream>

/** @defgroup urban_group Urban geometric representation
*  This Program loads an urban scene and offers tools for its manipulation
*  @{
*/

/**
*  namespace urban is in four groups
*  @see io @link io The i/o interface @endlink
*  @see shadow @link shadow The shadow interface @endlink
*
*/

namespace urban
{
    namespace scene
    {
        /**
        * @brief Container class modelling urbans objects
        * It is based on CGAL::Polyhedron structure.
        */
        class Brick
        {
        public:
            /**
            * Default constructor.
            * @see Brick(Brick const& other)
            * @see Brick(Brick && other)
            * @see Brick(shadow::Mesh const& mesh)
            * @see ~Brick(void)
            */
            Brick(void);
            /**
            * Copy constructor.
            * @param other an other brick
            * @see Brick(void)
            * @see Brick(Brick && other)
            * @see Brick(shadow::Mesh const& mesh)
            * @see ~Brick(void)
            */
            Brick(Brick const& other);
            /**
            * Move constructor.
            * @param other an other brick
            * @see Brick(void)
            * @see Brick(Brick const& other)
            * @see Brick(shadow::Mesh const& mesh)
            * @see ~Brick(void)
            */
            Brick(Brick && other);
            /**
            * Constructor from Shadow Mesh.
            * @param mesh Shadow Mesh
            * @param _reference_point reference point
            * @see Brick(void)
            * @see Brick(Brick const& other)
            * @see Brick(Brick && other)
            * @see ~Brick(void)
            */
            Brick(shadow::Mesh const& mesh, shadow::Point const& _reference_point);
            /**
            * Constructor from Shadow Mesh.
            * @param mesh Shadow Mesh
            * @param _reference_point reference point
            * @param _epsg_index epsg index
            * @see Brick(void)
            * @see Brick(Brick const& other)
            * @see Brick(Brick && other)
            * @see ~Brick(void)
            */
            Brick(shadow::Mesh const& mesh, shadow::Point const& _reference_point, unsigned short _epsg_index);
            /**
            * Default destructor.
            * @see Brick(void)
            * @see Brick(Brick const& other)
            * @see Brick(Brick && other)
            * @see Brick(shadow::Mesh const& mesh)
            */
            ~Brick(void);

            /**
            * Swap `this` with `other`.
            * @param other an other brick to swap with
            * @see swap(Brick & lhs, Brick & rhs)
            */
            void swap(Brick & other);
                
            /**
            * Copy assignement operator.
            * @param other an other brick to copy
            * @see operator=(Brick &&)
            */
            Brick & operator=(Brick const& other) noexcept;
            /**
            * Move assignement operator.
            * @param other an other brick to move
            * @see operator=(Brick const&)
            */
            Brick & operator=(Brick && other) noexcept;

            /**
            * Access brick name.
            * @return brick name
            */
            std::string get_name(void) const noexcept;
            /**
            * Get the bounding box
            * @return bounding box
            */
            Bbox_3 bbox(void) const noexcept;
            /**
            * Access brick epsg.
            * @return brick epsg
            */
            unsigned short get_epsg(void) const noexcept;
            /**
            * Access brick reference point.
            * @return brick reference point
            */
            shadow::Point get_reference_point(void) const noexcept;
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

            /** Halfedge handle */
            typedef Polyhedron::Halfedge_handle Halfedge_handle;
            /** Halfedge constant handle */
            typedef Polyhedron::Halfedge_const_handle Halfedge_const_handle;
            /** Halfedge iterator*/
            typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
            /** Halfedge constant iterator*/
            typedef Polyhedron::Halfedge_const_iterator Halfedge_const_iterator;

            /** Facet */
            typedef Polyhedron::Facet Facet;
            /** Facet handle */
            typedef Polyhedron::Facet_handle Facet_handle;
            /** Facet constant handle */
            typedef Polyhedron::Facet_const_handle Facet_const_handle;
            /** Facet iterator */
            typedef Polyhedron::Facet_iterator Facet_iterator;
            /** Facet constant iterator */
            typedef Polyhedron::Facet_const_iterator Facet_const_iterator;

            /** Points iterator */
            typedef Polyhedron::Point_iterator Point_iterator;
            /** Pointsc constant iterator */
            typedef Polyhedron::Point_const_iterator Point_const_iterator;

            /** Plane iterator */
            typedef Polyhedron::Plane_iterator Plane_iterator;
            /** Plane constant iterator */
            typedef Polyhedron::Plane_const_iterator Plane_const_iterator;

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

            /**
            * Compute the centroid of a brick facet
            * @param facet a brick facet
            * @return centroid of the facet
            */
            Point_3 centroid(const Brick::Facet & facet) const;
            /**
            * Compute the normal of a brick facet
            * @param facet a brick facet
            * @return the normal of the facet
            */
            Vector_3 normal(const Brick::Facet & facet) const;
            /**
            * Compute the area of a brick facet
            * @param facet a brick facet
            * @return area of the facet
            */
            double area(const Brick::Facet & facet) const;

            /** 
            * Finds a joinable halfedge.
            * @return a halfedge handle of joinable facets
            */
            Brick::Halfedge_iterator prunable(void);

            /** 
            * Finds all joinable halfedges.
            * @param facet a brick facet
            * @return a vector of pruning halfedge handles for the facet
            */
            std::vector<Brick::Halfedge_handle> combinable(Facet & facet) const;

            /** 
            * Clusters all facets into prunable facet bags.
            * @return a vector of all pruning halfedges
            */
            std::vector<Brick::Halfedge_handle> pruning_halfedges(void);

            /** 
            * Join prunable halfedge
            * Wraps CGAL join_facet() for brick.
            * @param h halfedge handle to join its incidents facets
            * @return `this` brick modified
            */
            Brick & join_facet(Halfedge_handle & h);

            /** 
            * Get facets adjacent to a facet
            * @param facet a brick facet
            * @return handle to facets adjacent to facet
            */
            std::vector<Brick::Facet_const_handle> facet_adjacents(Brick::Facet const& facet) const;

            /** 
            * Get facet adjacency matrix (dual matrix)
            * @return the facet adjacency matrix
            */
            std::vector<bool> facet_adjacency_matrix(void) const;
            /** 
            * Write facet adjacency matrix to building matrix
            * @param matrix the matrix to write on
            * @param offset the index of the block where to write
            * @return the matrix
            */
            std::vector<bool> & facet_adjacency_matrix(std::vector<bool> & matrix, std::size_t offset) const;
        private:
            /** Brick name */
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
            * Outstreaming Brick in OFF format
            * @param os the output stream
            * @param brick the brick to stream
            * @return the output stream
            */
            friend std::ostream& operator<<(std::ostream & os, Brick const& brick);
            /**
            * Outstreaming the dual adjacency graph
            * @param as the output stream
            * @param brick the brick to stream
            * @return the output stream
            */
            friend io::Adjacency_stream& operator<<(io::Adjacency_stream & as, Brick const& brick);
            #ifdef CGAL_USE_GEOMVIEW
            /**
            * Outstreaming Brick to GeomView
            * @param gs the output stream
            * @param brick the brick to stream
            * @return the output stream
            */
            friend CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream & gs, Brick const& brick);
            #endif // CGAL_USE_GEOMVIEW
        };
        /** @} */ // end of urban

        /**
        * Swaps two bricks.
        * @param lhs left-hand Brick.
        * @param rhs right-hand Brick.
        */
        void swap(Brick & lhs, Brick &rhs);
    }
}
