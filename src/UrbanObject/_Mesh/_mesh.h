#pragma once

#include "../urban_object.h"
#include "../Triangle/triangle.h"

#include <map>

namespace UrbanObject
{
    class _Mesh
    {
    public:
        _Mesh(void);
        _Mesh(const _Mesh &);
        _Mesh(Lib3dsMesh);
        ~_Mesh(void);

        void check_orientations(void);
    private:
        std::map<size_t, Point> points;
        std::map<size_t, Triangle> triangles;
    };
}
