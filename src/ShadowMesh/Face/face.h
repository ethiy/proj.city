#pragma once

/**
 * \file face.h
 * \brief Shadow Face definition
 * \author Oussama Ennafii
 * \version 0.0.0
 * \date 8 february 2017
 */

#include "../../geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <vector>
#include <ostream>

namespace urban
{

    namespace shadow
    {
        /** 
         * \class Face
         * \brief Face class representing a 3D facet.
         * 
         * Shadow Face is member class of Shadow Mesh. 
         * Shadow Face is member class of Shadow Mesh. 
         *  - It stores indexes for points that define a face.
         *  - Point coordinates are stored in the Mesh class.
         */
        class Face
        {
        public:
            
            /**
             * Empty Face constructor.
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            Face(void);
            
            /**
             * Copy constructor.
             * @param other a Face
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            Face(const Face &);
            
            /**
             * Move constructor.
             * @param other a Face
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            Face(Face &&);
            
            /**
             * General constructor. 
             * @throws std::out_of_range
             * @param _vertices_number facet degree
             * @param _points point indexes
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            Face(size_t, const std::vector<size_t> &);
            
            /**
             * Constructs a triagular face. 
             * @param first point index
             * @param second point index
             * @param third point index
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            Face(size_t, size_t, size_t);
            
            /** 
             * Destructor.
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            ~Face(void);

            /**
             * Swap `this` with `other`.
             * @param other an other face to swap with
             * @see swap(shadow::Face &, shadow::Face &)
             */
            void swap(Face &);
            
            /**
             * Copy assignement operator.
             * @param other an other face to copy
             * @see operator=(Face &&)
             */

            Face & operator=(const Face &) noexcept;
            /**
             * Move assignement operator.
             * @param other an other face to copy
             * @see operator=(const Face &)
             */
            Face & operator=(Face &&) noexcept;
            
            /**
             * Access operator[].
             * @param index point index to access
             */
            size_t operator[](size_t);

            /**
             * Access Face size.
             * @return facet degree
             */
            size_t size(void) const noexcept;

            /**
             * Iterator over face vertices.
             */
            typedef std::vector<size_t>::iterator iterator;

            /**
             * Constant iterator over face vertices.
             */
            typedef std::vector<size_t>::const_iterator const_iterator;

            /**
             * begin iterator access.
             * @return begin iterator
             */
            iterator begin(void) noexcept;

            /**
             * end iterator access.
             * @return end iterator
             */
            iterator end(void) noexcept;

            /**
             * begin iterator access.
             * @return constant begin iterator
             */
            const_iterator cbegin(void) const noexcept;

            /**
             * end iterator access.
             * @return constant end iterator
             */
            const_iterator cend(void) const noexcept;

            /**
             * Invert face orientation
             */
            void invert_orientation(void);
            
            /**
             * Evaluates if face is convex based on points coordinates.
             * @param coordinates map associating point indexes to their coordinates
             * @return facet convexity
             */
            bool is_convex(const std::map<size_t, Point> &) const;

            /**
             * Returns 3ds face structure.
             * @param coordinates map associating point indexes to their coordinates
             * @return pointer to `Lib3dsFace`
             * @throws std::logic_error
             */
            Lib3dsFace* to_3ds(const std::map<size_t, Point> &);
        private:
            size_t vertices_number;     /**< Face degree */
            std::vector<size_t> points; /**< Points array */

            /** 
             * Writes Face to output stream.
             * @param os output stream
             * @param face the facet to write
             * @return the output stream
             */
            friend std::ostream& operator<<(std::ostream &, const Face &);
        };
    }

    /**
     * Swaps two faces.
     * @param lhs left-hand Face.
     * @param rhs right-hand Face.
     */
    void swap(shadow::Face &, shadow::Face &);
}
