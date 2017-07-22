#pragma once

/**
 * \file face.h
 * \brief Shadow Face definition
 * \author Oussama Ennafii
 * \date 8 february 2017
 */

#include "../Point/point.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <vector>
#include <map>
#include <initializer_list>
#include <iostream>

namespace urban
{
    namespace shadow
    {
        /** 
         * @ingroup shadow_group
         * @brief Face class representing a 3D facet.
         * 
         * Shadow Face is member class of Shadow Mesh:
         *  - It stores indexes for points that define a face.
         *  - Point coordinates are stored in the Mesh class.
         */
        class Face
        {
        public:
            /**
             * Empty Face constructor.
             * @see Face(Face const& other);
             * @see Face(Face && other);
             * @see Face(std::initializer_list<std::size_t> initializer);
             * @see Face(std::vector const& indices);
             * @see Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
             * @see ~Face(void)
             */
            Face(void);
            /**
             * Copy constructor.
             * @param other a Face
             * @see Face(void);
             * @see Face(Face && other);
             * @see Face(std::initializer_list<std::size_t> initializer);
             * @see Face(std::vector const& indices);
             * @see Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
             * @see ~Face(void)
             */
            Face(Face const& other);
            /**
             * Move constructor.
             * @param other a Face
             * @see Face(void);
             * @see Face(Face const&)
             * @see Face(std::initializer_list<std::size_t> initializer);
             * @see Face(std::vector const& indices);
             * @see Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
             * @see ~Face(void)
             */
            Face(Face && other);
            /**
             * Initializer list constructor. 
             * @throws std::logic_error
             * @param initializer initializer list
             * @see Face(void);
             * @see Face(Face const&);
             * @see Face(Face &&);
             * @see Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
             * @see ~Face(void);
             */
            Face(std::initializer_list<std::size_t> initializer);
            /**
             * STL Vector constructor. 
             * @throws std::logic_error
             * @param indices vector containing point indices
             * @see Face(void);
             * @see Face(Face const&);
             * @see Face(Face &&);
             * @see Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
             * @see Face(std::initializer_list<std::size_t> initializer);
             * @see Face(std::vector const& indices);
             * @see ~Face(void);
             */
            Face(std::vector<std::size_t> const& indices);
            /**
             * Triangular face constructor. 
             * @param first point index
             * @param second point index
             * @param third point index
             * @see Face(void);
             * @see Face(Face const& other);
             * @see Face(Face && other);
             * @see Face(std::initializer_list<std::size_t> initializer);
             * @see Face(std::vector const& indices);
             * @see ~Face(void);
             */
            Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
            /** 
             * Destructor.
             * @see Face(void)
             * @see Face(Face const& other)
             * @see Face(Face && other)
             * @see Face(std::initializer_list<std::size_t> initializer)
             * @see Face(std::size_t first, std::size_t second, std::size_t third, bool orientation);
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
            Face & operator=(Face const& other) noexcept;
            /**
             * Move assignement operator.
             * @param other an other face to copy
             * @see operator=(Face const&)
             */
            Face & operator=(Face && other) noexcept;
            
            /**
             * Access operator[].
             * @param index point index to access
             * @return reference to the requested element
             */
            std::size_t & operator[](std::size_t index);
            /**
             * Access operator[].
             * @param index point index to access
             * @return constant reference to the requested element
             */
            std::size_t const& operator[](std::size_t index) const;
            
            /**
             * Access Facet degree.
             * @return facet degree
             */
            std::size_t get_degree(void) const noexcept;

            /** Iterator over face vertices */
            typedef std::vector<std::size_t>::iterator iterator;
            /** Constant iterator over face vertices */
            typedef std::vector<std::size_t>::const_iterator const_iterator;

            /**
             * Access begin iterator.
             * @return begin iterator
             */
            iterator begin(void) noexcept;
            /**
             * Access end iterator.
             * @return end iterator
             */
            iterator end(void) noexcept;
            /**
             * Access constant begin iterator.
             * @return begin constant iterator
             */
            const_iterator begin(void) const noexcept;
            /**
             * Access constant end iterator.
             * @return end constant iterator
             */
            const_iterator end(void) const noexcept;
            /**
             * Access begin constant iterator.
             * @return constant begin iterator
             */
            const_iterator cbegin(void) const noexcept;
            /**
             * Access end constant iterator.
             * @return constant end iterator
             */
            const_iterator cend(void) const noexcept;

            /**
             * Invert face orientation
             */
            void invert_orientation(void);
            /**
             * Apply a left rotation
             */
            void rotate(void);
            /**
             * Apply a left rotation n times
             * n number of left rotations
             */
            void rotate(long n);

            /**
             * Find an index.
             * @param index the index to find inside the facet
             * @return iterator to the index (it sould be unique)
             */
            iterator find(std::size_t index);
            /**
             * Find an index and overide it.
             * @param old_index the index to overide
             * @param new_index the index to overide with
             * @return true if successful
             */
            bool overide(std::size_t old_index, std::size_t new_index);

            /**
             * Evaluates if facet is convex based on points coordinates.
             * @param coordinates map associating point indexes to their coordinates
             * @return true if facet is convex
             */
            bool is_convex(std::map<std::size_t, Point> const& coordinates) const;

            /**
             * Writes to a 3ds face structure.
             * @param coordinates map associating point indexes to their coordinates
             * @return pointer to `Lib3dsFace` list
             * @throws std::logic_error
             */
            Lib3dsFace * to_3ds(std::map<std::size_t, Point> const& coordinates) const;
        private:
            /** Face degree */
            std::size_t degree;
            /** Points array */
            std::vector<std::size_t> points;

            /**
            * Check if two faces are equal:
            * if all indices are equal
            * @param lhs left-hand Face.
            * @param rhs right-hand Face.
            * @return boolean indicating if the two faces are equal
            */
            friend bool operator==(Face const& lhs, Face const& rhs);

            /** 
             * Writes Face to output stream.
             * @param os output stream
             * @param face the facet to write
             * @return reference to the output stream
             */
            friend std::ostream & operator<<(std::ostream & os, Face const& face);
        };

        /**
        * Check if two faces are different:
        * if at least two indices are different
        * @param lhs left-hand Face.
        * @param rhs right-hand Face.
        * @return boolean indicating if the two faces are different
        */
        bool operator!=(Face const& lhs, Face const& rhs);

        /**
        * Swaps two faces.
        * @param lhs left-hand Face.
        * @param rhs right-hand Face.
        */
        void swap(Face & lhs, Face & rhs);
    }
}
