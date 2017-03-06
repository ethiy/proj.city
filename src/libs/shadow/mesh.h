#pragma once

/**
 * \file mesh.h
 * \brief Shadow Mesh definition
 * \author Oussama Ennafii
 * \date 8 february 2017
 */


#include "Face/face.h"
#include "Bbox/bbox.h"
#include "Point/point.h"
#include "Vector/vector.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>
#include "../geometry_definitions.h"

#include <map>
#include <string>
#include <ostream>

namespace urban
{
    /** @defgroup shadow_group Shadow Structures
    *  This an interface to external structures
    *  @{
    */

    /**
    *  namespace shadow is in four groups
    *  @see @link shadow_group The i/o interface @endlink
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
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(size_t, const std::vector<size_t> &)
             * @see Mesh(size_t, size_t, size_t);
             * @see ~Mesh(void)
             */
            Mesh(void);

            /**
             * Copy constructor.
             * @param other a Mesh
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(const Mesh & other);

            /**
             * Move constructor.
             * @param other a Mesh
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(Mesh && other);

            /**
             * Constructor cconverting from Lib3dsMesh.
             * @param lib3ds_mesh lib3ds mesh structure
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(Lib3dsMesh* lib3ds_mesh);

            /**
             * General constructor. 
             * @param polyhedron Polyhedron_3 structure
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(const Polyhedron & polyhedron);

            /**
             * General constructor. 
             * @param _name mesh name
             * @param _points points coordinates
             * @param _faces facets
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(std::string _name, const std::map<size_t, Point> & _points, const std::map<size_t, Face> & _faces);
            
            /**
             * General constructor. 
             * @param _name mesh name
             * @param _points points coordinates
             * @param _faces facets
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(std::string _name, const std::map<size_t, Point> & _points, const std::map<size_t, Face> & _faces, const Bbox & _bounding_box);

            /** 
             * Destructor.
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
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
             * @see operator=(Mesh &&)
             */
            Mesh & operator=(const Mesh & other) noexcept;


            /**
             * Move assignement operator.
             * @param other an other face to copy
             * @see operator=(const Mesh &)
             */
            Mesh & operator=(Mesh && other) noexcept;


            /**
             * Points iterator
             */
            typedef std::map<size_t, Point>::iterator points_iterator;

            /**
             * Points constant iterator
             */
            typedef std::map<size_t, Point>::const_iterator points_const_iterator;
            
            /**
             * Faces iterator
             */
            typedef std::map<size_t, Face>::iterator faces_iterator;

            /**
             * Faces constant iterator
             */
            typedef std::map<size_t, Face>::const_iterator faces_const_iterator;

            /**
             * Access begin points iterator
             * @return begin points iterator
             */
            points_iterator points_begin(void) noexcept;
            
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
            

            /**
             * Access Mesh name
             * @return mesh name
             */
            std::string get_name(void) const noexcept;

            /**
             * Access number of points.
             * @return number of points defining the mesh
             */
            size_t points_size(void) const noexcept;

            /**
             * Access Mesh coordinates.
             * @return Mesh coordinates map
             */
            std::map<size_t, Point> get_points(void) const noexcept;

            /**
             * Access number of facets.
             * @return number of facets defining the mesh
             */
            size_t faces_size(void) const noexcept;
            
            /**
             * Access Mesh facets.
             * @return facets defining the mesh
             */
            std::map<size_t, Face> get_faces(void) const noexcept;


            /**
             * Access The bounding box.
             * @return the bounding box of the mesh
             */
            Bbox bbox(void) const noexcept;

            /** 
             * Access index of halfedge.
             * logarithmic complexity due to the use of std::map
             * @param h halfedge to lookup
             * @return index of the halfedge
             */
            size_t get_index(const Polyhedron::Halfedge & h);

            /**
             * Returns 3ds mesh structure.
             * @return pointer to `Lib3dsMesh`
             */
            Lib3dsMesh* to_3ds(void);
        private:
            /** Mesh name*/
            std::string name;
            /** Coordinates*/
            std::map<size_t, Point> points;
            /** Faces*/
            std::map<size_t, Face> faces;
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
            friend std::ostream& operator<<(std::ostream & os, const Mesh & mesh);
        };

        /**
        * Comparison operator
        * @param lhs left-hand Mesh.
        * @param rhs right-hand Mesh.
        $ @return true if both Meshes are equal
        */
        bool operator==(const shadow::Mesh & lhs, const shadow::Mesh & rhs);

        /**
        * Comparison operator
        * @param lhs left-hand Mesh.
        * @param rhs right-hand Mesh.
        $ @return true if both Meshes are inequal
        */
        bool operator!=(const shadow::Mesh & lhs, const shadow::Mesh & rhs);
    }
    /** @} */ // end of shadow_group

    /**
     * Swaps two meshes.
     * @param lhs left-hand Mesh.
     * @param rhs right-hand Mesh.
     */
    void swap(shadow::Mesh & lhs, shadow::Mesh & rhs);
}
