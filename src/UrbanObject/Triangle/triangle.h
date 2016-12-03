#pragma once

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
        
        size_t operator[](size_t);

        void invert_orientation(void);
    private:
        std::array<size_t, 3> points;
        friend std::ostream& operator<<(std::ostream &, const Triangle &);
    };
}
