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
        Lib3dsMesh* mesh = reinterpret_cast<Lib3dsMesh*>(calloc(sizeof(Lib3dsMesh), 1));
        mesh->points = static_cast<Lib3dsWord>(points.size());
        mesh->pointL = reinterpret_cast<Lib3dsPoint*>(calloc(sizeof(Lib3dsPoint), mesh->points));
        std::transform(std::begin(points), std::end(points), mesh->pointL, [&](std::pair<size_t, Point> p)
                                                                                            {
                                                                                                Lib3dsPoint point;
                                                                                                auto init = std::initializer_list<double>({p.second[0], p.second[1], p.second[2]});
                                                                                                std::copy(std::begin(init), std::end(init), point.pos);
                                                                                                return point;
                                                                                            }
                    );
        mesh->faces = static_cast<Lib3dsDword>(triangles.size());
        mesh->faceL = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), mesh->faces));
        std::transform(std::begin(triangles), std::end(triangles), mesh->faceL, [&](std::pair<size_t, Triangle> t)
                                                                                    {
                                                                                        return *t.second.to_3ds();
                                                                                    }
                    );
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
