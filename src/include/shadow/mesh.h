#pragma once

/**
 * \file mesh.h
 * \brief Shadow Mesh definition
 * \author Oussama Ennafii
 * \date 8 february 2017
 */


#include <shadow/face.h>
#include <shadow/bbox.h>
#include <shadow/point.h>
#include <shadow/vector.h>

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>
#include <geometry_definitions.h>

#include <map>
#include <string>
#include <ostream>

namespace city
{
    namespace scene
    {
        class UNode;
    }
    /** @defgroup shadow_group Shadow Structures
    *  This is an interface to external structures
    *  @{
    */

    /**
    *  namespace shadow is in four groups
    *  @see io @link io The i/o interface @endlink
    *
    */
    namespace shadow
    {
        /** 
         * @brief Mesh class representing a 3D surface.
         * Lightweight face-based data structure representing 3D surfaces. Its role is to interface between the input/output utility that depends on external datastructures and the internal objects.
         */
        class Mesh
        {
            
        public:

            /**
             * Empty Mesh constructor.
             * @see Mesh(Mesh && other);
             * @see Mesh(Mesh const& other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(void);
            /**
             * Copy constructor.
             * @param other a Mesh
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(Mesh const& other);
            /**
             * Move constructor.
             * @param other a Mesh
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Mesh const& other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(Mesh && other);
            /**
             * Constructor converting from Lib3dsMesh.
             * @param lib3ds_mesh lib3ds mesh structure
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Mesh const& other);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(Lib3dsMesh* lib3ds_mesh);
            /**
             * General constructor. 
             * @param polyhedron Polyhedron_3 structure
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(std::string const& _name, Polyhedron const& polyhedron);
            Mesh(scene::UNode const& unode);
            /**
             * General constructor. 
             * @param _name mesh name
             * @param _points points coordinates
             * @param _faces facets
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Mesh const& other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(std::string const& _name, std::vector<Point> const& _points, std::vector<Face> const& _faces);
            /**
             * General constructor. 
             * @param _points points coordinates
             * @param _faces facets
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Mesh const& other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            Mesh(std::vector<Point> const& _points, std::vector<Face> const& _faces);
            /** 
             * Destructor.
             * @see Mesh(void);
             * @see Mesh(Mesh && other);
             * @see Mesh(Mesh const& other);
             * @see Mesh(Lib3dsMesh* lib3ds_mesh);
             * @see Mesh(Polyhedron const& polyhedron);
             * @see Mesh(std::string const& _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const & _faces);
             * @see ~Mesh(void);
             */
            ~Mesh(void);

            /**
             * Swap `this` with `other`.
             * @param other an other mesh to swap with
             * @see swap(shadow::Mesh &, shadow::Mesh &)
             */
            void swap(Mesh & other);
            /**
             * Copy assignement operator.
             * @param other an other mesh to copy
             * @see operator =(Mesh &&)
             */
            Mesh & operator =(Mesh const& other) noexcept;
            /**
             * Move assignement operator.
             * @param other an other face to copy
             * @see operator =(Mesh const&)
             */
            Mesh & operator =(Mesh && other) noexcept;


            /** Points iterator */
            using points_iterator = std::vector<Point>::iterator;
            /** Points constant iterator */
            using points_const_iterator = std::vector<Point>::const_iterator;
            /** Faces iterator */
            using faces_iterator = std::vector<Face>::iterator;
            /** Faces constant iterator */
            using faces_const_iterator = std::vector<Face>::const_iterator;

            /**
             * Access begin points iterator
             * @return begin points iterator
             */
            points_iterator points_begin(void) noexcept;
            points_const_iterator points_begin(void) const noexcept;
            /**
             * Access constant begin points iterator
             * @return constant begin points iterator
             */
            points_const_iterator points_cbegin(void) const noexcept;
            /**
             * Access end points iterator
             * @return end points iterator
             */
            points_iterator points_end(void) noexcept;
            points_const_iterator points_end(void) const noexcept;
            /**
             * Access constant end points iterator
             * @return constant end points iterator
             */
            points_const_iterator points_cend(void) const noexcept;
            /**
             * Access begin faces iterator
             * @return begin faces iterator
             */
            faces_iterator faces_begin(void) noexcept;
            /**
             * Access constant begin faces iterator
             * @return constant begin faces iterator
             */
            faces_const_iterator faces_cbegin(void) const noexcept;
            /**
             * Access end faces iterator
             * @return end faces iterator
             */
            faces_iterator faces_end(void) noexcept;
            /**
             * Access constant end faces iterator
             * @return constant end faces iterator
             */
            faces_const_iterator faces_cend(void) const noexcept;
            bool is_empty(void) const noexcept;

            /**
             * Name the mesh
             */
            Mesh set_name(std::string const& _name) noexcept;

            /**
             * Access Mesh name
             * @return mesh name
             */
            std::string get_name(void) const noexcept;
            /**
             * Access number of points.
             * @return number of points defining the mesh
             */
            std::size_t points_size(void) const noexcept;
            /**
             * Access Mesh coordinates.
             * @return Mesh coordinates map
             */
            std::vector<Point> const& get_points(void) const noexcept;
            std::vector<Point> & get_points(void) noexcept;
            std::vector<Point_3> get_cgal_points(void) const noexcept;
            /**
             * Access number of facets.
             * @return number of facets defining the mesh
             */
            std::size_t faces_size(void) const noexcept;
            /**
             * Access Mesh facets.
             * @return facets defining the mesh
             */
            std::vector<Face> const& get_faces(void) const noexcept;
            std::vector<Face> & get_faces(void) noexcept;
            std::vector< std::vector<std::size_t> > get_cgal_faces(void) const noexcept;
            /**
             * Access The bounding box.
             * @return the bounding box of the mesh
             */
            Bbox const& bbox(void) const noexcept;

            Mesh & operator +=(Mesh const& other);

            /**
             * Returns 3ds mesh structure.
             * @return pointer to `Lib3dsMesh`
             */
            Lib3dsMesh* to_3ds(void) const;
        private:
            /** Mesh name*/
            std::string name;
            /** Coordinates*/
            std::vector<Point> points;
            /** Faces*/
            std::vector<Face> faces;
            /** Bounding box*/
            Bbox bounding_box;

            /** Compute bounding box internal method*/
            void compute_bbox(void);

            /** 
             * Writes Mesh to output stream.
             * @param os output stream
             * @param mesh the Mesh to write
             * @return the output stream
             */
            friend std::ostream& operator <<(std::ostream & os, Mesh const& mesh);
        };

        Mesh operator +(Mesh const& lhs, Mesh const& rhs);

        /**
        * Comparison operator 
        * @param lhs left-hand Mesh.
        * @param rhs right-hand Mesh.
        $ @return true if both Meshes are equal
        */
        bool operator ==(Mesh const& lhs, Mesh const& rhs);

        /**
        * Comparison operator 
        * @param lhs left-hand Mesh.
        * @param rhs right-hand Mesh.
        $ @return true if both Meshes are inequal
        */
        bool operator !=(Mesh const& lhs, Mesh const& rhs);
    }
    /** @} */ // end of shadow_group

    /**
     * Swaps two meshes.
     * @param lhs left-hand Mesh.
     * @param rhs right-hand Mesh.
     */
    void swap(shadow::Mesh & lhs, shadow::Mesh & rhs);
}
