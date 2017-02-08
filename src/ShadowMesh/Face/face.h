#pragma once

#include "../../geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <vector>
#include <ostream>

namespace urban
{
    class Face
    {
    public:
        /*! Empty Face constructor*/
        Face(void);
        /*! Copy constructor*/
        Face(const Face &);
        /*! Move */
        Face(Face &&);
        Face(size_t, const std::vector<size_t> &);
        Face(size_t, size_t, size_t);
        ~Face(void);

        void swap(Face &);
        
        Face & operator=(Face) noexcept;
        Face & operator=(Face &&) noexcept;
        size_t operator[](size_t);

        size_t size(void) const noexcept;
        typedef std::vector<size_t>::iterator iterator;
        typedef std::vector<size_t>::const_iterator const_iterator;
        iterator begin(void) noexcept;
        iterator end(void) noexcept;
        const_iterator cbegin(void) const noexcept;
        const_iterator cend(void) const noexcept;

        void invert_orientation(void);
        bool is_convex(const std::map<size_t, Point> &) const;
        Lib3dsFace* to_3ds(const std::map<size_t, Point> &);
    private:
        size_t vertices_number;
        std::vector<size_t> points;

        friend std::ostream& operator<<(std::ostream &, const Face &);
    };

    void swap(Face &, Face &);
}
