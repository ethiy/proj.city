#pragma once

#include "../../geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <vector>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Face
        {
        public:
            /*! Empty Face constructor*/
            Face(void);
            /*! Copy constructor*/
            Face(const Face &);
            /*! Move constructor*/
            Face(Face &&);
            /*! General constructor
            *      throws error if there less than 3 points
            */
            Face(size_t, const std::vector<size_t> &);
            /*! Constructs a triagular face*/
            Face(size_t, size_t, size_t);
            /*! Destructor*/
            ~Face(void);

            /*! Swap `this` with `other`*/
            void swap(Face &);
            
            /*! Assignement operator*/
            Face & operator=(Face) noexcept;
            /*! Move assignement operator*/
            Face & operator=(Face &&) noexcept;
            /*! Access operator*/
            size_t operator[](size_t);

            /*! Access Face size*/
            size_t size(void) const noexcept;
            /*! Iterators over face vertices*/
            typedef std::vector<size_t>::iterator iterator;
            typedef std::vector<size_t>::const_iterator const_iterator;
            /*! begin and end iterators access functions*/
            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            /*! Invert face orientation*/
            void invert_orientation(void);
            
            /*! Evaluate if face is convex based on points coordinates*/
            bool is_convex(const std::map<size_t, Point> &) const;

            /*! Returns 3ds face structure*/
            Lib3dsFace* to_3ds(const std::map<size_t, Point> &);
        private:
            /*! Face degree*/
            size_t vertices_number;
            /*! Points array*/
            std::vector<size_t> points;

            /*! write Face to stream*/
            friend std::ostream& operator<<(std::ostream &, const Face &);
        };
    }

    /*! Swaps two faces*/
    void swap(shadow::Face &, shadow::Face &);
}
