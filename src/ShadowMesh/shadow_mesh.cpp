#include "shadow_mesh.h"

#include <iterator>
#include <algorithm>
#include <numeric>

namespace urban
{
    ShadowMesh::ShadowMesh(void):name("N/A"){}
    ShadowMesh::ShadowMesh(const ShadowMesh & other):name(other.name), points(other.points), faces(other.faces){}
    ShadowMesh::ShadowMesh(Lib3dsMesh* lib3ds_mesh):name(lib3ds_mesh->name)
    {
        size_t it(0);
        std::for_each(
            lib3ds_mesh->pointL,
            lib3ds_mesh->pointL + lib3ds_mesh->points,
            [&](Lib3dsPoint _point)
            {
                points[it++] = Point(_point.pos[0], _point.pos[1], _point.pos[2]);
            }
        );

        it = 0;
        std::for_each(
            lib3ds_mesh->faceL,
            lib3ds_mesh->faceL + lib3ds_mesh->faces,
            [&](Lib3dsFace _face)
            {
                Point point_0 = points[_face.points[0]];
                Point point_1 = points[_face.points[1]];
                Point point_2 = points[_face.points[2]];

                Vector v1 = Vector(point_0, point_1);
                Vector v2 = Vector(point_1, point_2);
                Vector n = Vector(_face.normal[0], _face.normal[1], _face.normal[2]);

                if(CGAL::determinant(v1, v2, n)>0)
                    faces[it++] = Face(_face.points[0], _face.points[1], _face.points[2]);
                else
                    faces[it++] = Face(_face.points[0], _face.points[2], _face.points[1]);
            }
        );
    }
    ShadowMesh::ShadowMesh(std::string _name, std::map<size_t, Point>_points, std::map<size_t, Face> _faces):name(_name), points(_points), faces(_faces){}
    ShadowMesh::~ShadowMesh(void){}

    void ShadowMesh::swap(ShadowMesh & other)
    {
        using std::swap;
        swap(name, other.name);
        swap(points, other.points);
        swap(faces, other.faces);
    }

    ShadowMesh & ShadowMesh::operator=(ShadowMesh other)
    {
        other.swap(*this);
        return *this;
    }

    std::string ShadowMesh::get_name(void) const noexcept
    {
        return name;
    }

    size_t ShadowMesh::get_number_points(void) const noexcept
    {
        return points.size();
    }

    std::map<size_t, Point> ShadowMesh::get_points(void) const noexcept
    {
        return points;
    }
    
    size_t ShadowMesh::get_number_faces(void) const noexcept
    {
        return faces.size();
    }

    std::map<size_t, Face> ShadowMesh::get_faces(void) const noexcept
    {
        return faces;
    }

    Lib3dsMesh* ShadowMesh::to_3ds()
    {
        Lib3dsMesh* mesh = reinterpret_cast<Lib3dsMesh*>(calloc(sizeof(Lib3dsMesh), 1));
        mesh->points = static_cast<Lib3dsWord>(points.size());
        mesh->pointL = reinterpret_cast<Lib3dsPoint*>(calloc(sizeof(Lib3dsPoint), mesh->points));

        std::transform(
            std::begin(points),
            std::end(points),
            mesh->pointL,
            [&](std::pair<size_t, Point> p)
            {
                Lib3dsPoint point;
                auto init = std::initializer_list<double>({p.second[0], p.second[1], p.second[2]});
                std::copy(std::begin(init), std::end(init), point.pos);
                return point;
            }
        );

        mesh->faces = static_cast<Lib3dsDword>(
            std::accumulate(
                std::begin(faces), 
                std::end(faces),
                0,
                [](int& size, std::pair<const size_t, Face> & second)
                {
                    return size + static_cast<int>(second.second.size()) - 2;
                }
                )
            );

        mesh->faceL = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), mesh->faces));

        std::transform(
            std::begin(faces),
            std::end(faces),
            mesh->faceL,
            [](std::pair<size_t, Face> t)
            {
                return *t.second.to_3ds();
            }
        );

        mesh->next = NULL;

        return mesh;
    }

    std::ostream& operator<<(std::ostream &os, const ShadowMesh & mesh)
    {
        os << "Name: " << mesh.name << std::endl
           << "Points: " << std::endl;

        std::for_each(
            std::begin(mesh.points),
            std::end(mesh.points),
            [&](std::pair<size_t, Point> p)
            {
                os << "Point " << p.first << " : " << p.second << std::endl;
            }
        );

        os << "Faces: " << std::endl;

        std::for_each(
            std::begin(mesh.faces),
            std::end(mesh.faces),
            [&](std::pair<size_t, Face> t)
            {
                os << "Face " << t.first << " : " << t.second << std::endl;
            }
        );

        return os;
    }

    void swap(ShadowMesh & lhs, ShadowMesh & rhs)
    {
        lhs.swap(rhs);
    }
}
