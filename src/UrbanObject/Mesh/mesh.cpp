#include "mesh.h"

#include <iterator>
#include <algorithm>

namespace urban
{
    Mesh::Mesh(void){}
    Mesh::Mesh(const Mesh & other):points(other.points), triangles(other.triangles){}
    Mesh::Mesh(Lib3dsMesh lib3ds_mesh)
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
    Mesh::~Mesh(void){}

    size_t Mesh::get_number_points(void)
    {
        return points.size();
    }

    std::map<size_t, Point> Mesh::get_points(void)
    {
        return points;
    }
    
    size_t Mesh::get_number_triangles(void)
    {
        return triangles.size();
    }

    std::map<size_t, Triangle> Mesh::get_triangles(void)
    {
        return triangles;
    }

    Lib3dsMesh* Mesh::to_3ds()
    {
        Lib3dsMesh* mesh = lib3ds_mesh_new("");
        lib3ds_mesh_new_point_list(mesh, static_cast<Lib3dsDword>(points.size()));
        lib3ds_mesh_new_face_list(mesh, LIB3DS_FACE_FLAG_VIS_AB | LIB3DS_FACE_FLAG_VIS_BC | LIB3DS_FACE_FLAG_VIS_AC);
        for(Lib3dsDword it=0; it<mesh->points; ++it)
        {
            Lib3dsPoint *point = reinterpret_cast<Lib3dsPoint*>(calloc(sizeof(Lib3dsPoint),1));
            for( size_t i=0; i<3; ++i)
                point->pos[i] = static_cast<float>(points[it][static_cast<int>(i)]);
                *(mesh->pointL + it) = *point;
        }
        mesh->faces = static_cast<Lib3dsDword>(triangles.size());
        for(Lib3dsDword it=0; it<mesh->faces; ++it)
            *(mesh->faceL + it) = *(triangles[it].to_3ds());
        Lib3dsVector *normalL = reinterpret_cast<Lib3dsVector*>(calloc(sizeof(Lib3dsVector),mesh->faces));
        lib3ds_mesh_calculate_normals(mesh, normalL);
        return mesh;
    }

    std::ostream& operator<<(std::ostream &os, const Mesh & mesh)
    {
        os << "Points: " << std::endl;
        std::for_each(std::begin(mesh.points), std::end(mesh.points), [&](std::pair<size_t, Point> p)
                                                                        {
                                                                            os << "Point " << p.first << " : " << p.second << std::endl;
                                                                        }
                    );
        os << "Triangles: " << std::endl;
        std::for_each(std::begin(mesh.triangles), std::end(mesh.triangles), [&](std::pair<size_t, Triangle> t)
                                                                                {
                                                                                    os << "Triangle " << t.first << " : " << t.second << std::endl;
                                                                                }
                    );
        return os;
    }
}
