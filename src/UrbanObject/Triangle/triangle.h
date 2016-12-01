#pragma once

#include <tuple>

namespace UrbanObject
{
    class Triangle
    {
    public:
        Triangle(void);
        Triangle(const Triangle &);
        Triangle(size_t, size_t, size_t);
        ~Triangle(void);

        void invert_orientation(void);
    private:
        std::tuple<size_t, size_t, size_t> points;
    };
}
