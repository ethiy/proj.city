#pragma once

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <array>
#include <ostream>

namespace urban
{
    class Face
    {
    public:
        Face(void);
        Face(const Face &);
        Face(size_t, size_t, size_t);
        ~Face(void);

        void swap(Face &);
        
        Face & operator=(Face);
        size_t operator[](size_t);

        void invert_orientation(void);
        Lib3dsFace* to_3ds(void);
    private:
        std::array<size_t, 3> points;
        friend std::ostream& operator<<(std::ostream &, const Face &);
    };

    void swap(Face &, Face &);
}
