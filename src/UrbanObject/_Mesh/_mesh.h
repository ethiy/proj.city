#pragma once

#include "../Triangle/triangle.h"

#include <CGAL/Simple_cartesian.h>

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <map>

namespace urban
{
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;

    class _Mesh
    {
    public:
        _Mesh(void);
        _Mesh(const _Mesh &);
        _Mesh(Lib3dsMesh);
        ~_Mesh(void);

        std::map<size_t, Point> get_points(void);
        std::map<size_t, Triangle> get_triangles(void);
    private:
        std::map<size_t, Point> points;
        std::map<size_t, Triangle> triangles;
    };
}
