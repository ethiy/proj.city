#pragma once

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <vector>
#include <ostream>

namespace urban
{
    class Face
    {
    public:
        Face(void);
        Face(const Face &);
        Face(size_t, std::vector<size_t>);
        Face(size_t, size_t, size_t);
        ~Face(void);

        void swap(Face &);
        
        Face & operator=(Face);
        size_t operator[](size_t);

        size_t size(void);
        typedef std::vector<size_t>::iterator iterator;
        typedef std::vector<size_t>::const_iterator const_iterator;
        iterator begin(void) noexcept;
        iterator end(void) noexcept;
        const_iterator cbegin(void) const noexcept;
        const_iterator cend(void) const noexcept;

        void invert_orientation(void);
        Lib3dsFace* to_3ds(void);
    private:
        size_t vertices_number;
        std::vector<size_t> points;
        friend std::ostream& operator<<(std::ostream &, const Face &);
    };

    void swap(Face &, Face &);
}
