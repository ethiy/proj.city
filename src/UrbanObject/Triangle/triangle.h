#pragma once

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <array>
#include <ostream>

namespace urban
{
    class Triangle
    {
    public:
        Triangle(void);
        Triangle(const Triangle &);
        Triangle(size_t, size_t, size_t);
        ~Triangle(void);

        void swap(Triangle &);
        
        Triangle & operator=(Triangle);
        size_t operator[](size_t);

        void invert_orientation(void);
        Lib3dsFace* to_3ds(void);
    private:
        std::array<size_t, 3> points;
        friend std::ostream& operator<<(std::ostream &, const Triangle &);
    };

    void swap(Triangle &, Triangle &);
}
