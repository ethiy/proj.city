#include "_mesh.h"

#include <iterator>
#include <algorithm>

namespace urban
{
    _Mesh::_Mesh(void){}
    _Mesh::_Mesh(const _Mesh & other):points(other.points), triangles(other.triangles){}
    _Mesh::_Mesh(Lib3dsMesh lib3ds_mesh)
    {
        for(size_t it=0; it<lib3ds_mesh.points; ++it)
            points[it] = Point((lib3ds_mesh.pointL+it)->pos[0], (lib3ds_mesh.pointL+it)->pos[1], (lib3ds_mesh.pointL+it)->pos[2]);
        for(size_t it=0; it<lib3ds_mesh.faces; ++it)
        {
            Point point_0 = points[(lib3ds_mesh.faceL+it)->points[0]];
            Point point_1 = points[(lib3ds_mesh.faceL+it)->points[1]];
            Point point_2 = points[(lib3ds_mesh.faceL+it)->points[2]];

            Vector v1 = Vector(point_0, point_1);
            Vector v2 = Vector(point_1, point_2);
            Vector n = Vector((lib3ds_mesh.faceL+it)->normal[0], (lib3ds_mesh.faceL+it)->normal[1], (lib3ds_mesh.faceL+it)->normal[2]);

            if(CGAL::determinant(v1, v2, n)>0)
                triangles[it] = Triangle((lib3ds_mesh.faceL+it)->points[0], (lib3ds_mesh.faceL+it)->points[1], (lib3ds_mesh.faceL+it)->points[2]);
            else
                triangles[it] = Triangle((lib3ds_mesh.faceL+it)->points[0], (lib3ds_mesh.faceL+it)->points[2], (lib3ds_mesh.faceL+it)->points[1]);
        }
    }
    _Mesh::~_Mesh(void){}

    std::map<size_t, Point> get_points(void)
    {
        return points;
    }
    std::map<size_t, Triangle> get_triangles(void)
    {
        return triangles;
    }
}