#pragma once

#include "../Triangle/triangle.h"

#include <CGAL/Simple_cartesian.h>

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <map>
#include <ostream>

namespace urban
{
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;

    class Mesh
    {
    public:
        Mesh(void);
        Mesh(const Mesh &);
        Mesh(Lib3dsMesh);
        ~Mesh(void);

        size_t get_number_points(void);
        std::map<size_t, Point> get_points(void);
        size_t get_number_triangles(void);
        std::map<size_t, Triangle> get_triangles(void);
    private:
        std::map<size_t, Point> points;
        std::map<size_t, Triangle> triangles;
        
        friend std::ostream& operator<<(std::ostream &, const Mesh &);
    };
}
