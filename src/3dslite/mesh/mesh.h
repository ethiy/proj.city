#ifndef MESH_H
#define MESH_H

#include <CGAL/Simple_cartesian.h>

namespace lite3ds
{
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_3                Point;

    class Mesh
    {
        // Go through all faces and add them iteratively to the polyhedron_3
    };
}

#endif
