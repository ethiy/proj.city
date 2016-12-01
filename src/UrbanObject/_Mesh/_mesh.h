#pragma once

#include "../urban_object.h"
#include "../Triangle/triangle.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <map>

namespace urban
{
    class _Mesh
    {
    public:
        _Mesh(void);
        _Mesh(const _Mesh &);
        _Mesh(Lib3dsMesh);
        ~_Mesh(void);
    private:
        std::map<size_t, Point> points;
        std::map<size_t, Triangle> triangles;
    };
}
