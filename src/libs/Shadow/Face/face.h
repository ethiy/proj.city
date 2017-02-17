#pragma once

/**
 * \file face.h
 * \brief Shadow Face definition
 * \author Oussama Ennafii
 * \date 8 february 2017
 */

#include "../Vector/vector.h"
#include "../Point/point.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <vector>
#include <map>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        /** 
         * @ingroup shadow
         * @brief Face class representing a 3D facet.
         * 
         * Shadow Face is member class of Shadow Mesh. Shadow Face is member class of Shadow Mesh:
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
            Face(const Face & other);
            
            /**
             * Move constructor.
             * @param other a Face
             * @see Face(void)
             * @see Face(Face &&)
             * @see Face(size_t, const std::vector<size_t> &)
             * @see Face(size_t, size_t, size_t);
             * @see ~Face(void)
             */
            Face(Face && other);
            
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
            Face(size_t _vertices_number, const std::vector<size_t> & _points);
            
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
            Face(size_t first, size_t second, size_t third);
            
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
            void swap(Face & other);
            
            /**
             * Copy assignement operator.
             * @param other an other face to move
             * @see operator=(Face &&)
             */
            Face & operator=(const Face & other) noexcept;

            /**
             * Move assignement operator.
             * @param other an other face to copy
             * @see operator=(const Face &)
             */
            Face & operator=(Face && other) noexcept;
            
            /**
             * Access operator[].
             * @param index point index to access
             */
            size_t operator[](size_t index);
            
            /**
             * Substruct a face from the current one.
             * @param other other face to compare
             * @return result of substraction
             */
            Face & operator-=(const Face & other);

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
             * Find an index
             * @param index the index to find inside the facet
             * @return iterator to the index (it sould be unique)
             */
            iterator find(size_t index);
            
            /**
             * Find an index and overide it
             * @param index the index to overide
             * @param new_index the index to overide with
             * @return true in case of success
             */
            bool overide(size_t index, size_t new_index);

            /**
             * Evaluates if face is convex based on points coordinates.
             * @param coordinates map associating point indexes to their coordinates
             * @return facet convexity
             */
            bool is_convex(const std::map<size_t, Point> & coordinates) const;

            /**
             * Returns 3ds face structure.
             * @param coordinates map associating point indexes to their coordinates
             * @return pointer to `Lib3dsFace`
             * @throws std::logic_error
             */
            Lib3dsFace* to_3ds(const std::map<size_t, Point> & coordinates);
        private:
            size_t vertices_number;     /**< Face degree */
            std::vector<size_t> points; /**< Points array */

            /** 
             * Writes Face to output stream.
             * @param os output stream
             * @param face the facet to write
             * @return the output stream
             */
            friend std::ostream& operator<<(std::ostream & os, const Face & face);
        };
    }

    /**
     * Swaps two faces.
     * @param lhs left-hand Face.
     * @param rhs right-hand Face.
     */
    void swap(shadow::Face & lhs, shadow::Face & rhs);

    /**
     * Substruct operation on Faces
     * @param lhs left-hand Face.
     * @param rhs right-hand Face.
     * @return the result of lhs - rhs
     */
    shadow::Face & operator-(shadow::Face & lhs, const shadow::Face & rhs);

    /**
     * Check if two faces are equal:
     * if all indices are equal
     * @param lhs left-hand Face.
     * @param rhs right-hand Face.
     * @return boolean indicating if the two faces are equal
     */
    bool operator==(const shadow::Face & lhs, const shadow::Face & rhs);
    /**
     * Check if two faces are different:
     * if at least two indices are different
     * @param lhs left-hand Face.
     * @param rhs right-hand Face.
     * @return boolean indicating if the two faces are different
     */
    bool operator!=(const shadow::Face & lhs, const shadow::Face & rhs);
}
