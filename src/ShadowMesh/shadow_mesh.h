#pragma once

/**
 * \file shadow_mesh.h
 * \brief Shadow Mesh definition
 * \author Oussama Ennafii
 * \date 8 february 2017
 */


#include "Face/face.h"

#include "../geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

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
    *  @see @link shadow_group The first group@endlink, group2, group3, group4 
    *
    *  Also see @ref mypage2
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
            Mesh(const Mesh &);

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
            Mesh(Mesh &&);

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
            Mesh(Lib3dsMesh*);

            /**
             * General constructor. 
             * @param polyhedron CGAL Polyhedron_3 structure
             * @see Mesh(void)
             * @see Mesh(Mesh &&)
             * @see Mesh(Lib3dsMesh*)
             * @see Mesh(const Polyhedron &)
             * @see Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>)
             * @see ~Mesh(void)
             */
            Mesh(const Polyhedron &);

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
            Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>);

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
            void swap(Mesh &);


            /**
             * Copy assignement operator.
             * @param other an other mesh to copy
             * @see operator=(Mesh &&)
             */
            Mesh & operator=(const Mesh &) noexcept;


            /**
             * Move assignement operator.
             * @param other an other face to copy
             * @see operator=(const Mesh &)
             */
            Mesh & operator=(Mesh &&) noexcept;


            /**
             * Access Mesh name
             * @return mesh name
             */
            std::string get_name(void) const noexcept;

            /**
             * Access number of points.
             * @return number of points defining the mesh
             */
            size_t get_number_points(void) const noexcept;

            /**
             * Access Mesh coordinates.
             * @return Mesh coordinates map
             */
            std::map<size_t, Point> get_points(void) const noexcept;

            /**
             * Access number of facets.
             * @return number of facets defining the mesh
             */
            size_t get_number_faces(void) const noexcept;
            
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
            void compute_box(void);

            /** 
             * Writes Mesh to output stream.
             * @param os output stream
             * @param mesh the Mesh to write
             * @return the output stream
             */
            friend std::ostream& operator<<(std::ostream &, const Mesh &);
        };
    }
    /** @} */ // end of shadow_group

    /**
     * Swaps two meshes.
     * @param lhs left-hand Mesh.
     * @param rhs right-hand Mesh.
     */
    void swap(shadow::Mesh &, shadow::Mesh &);
}
