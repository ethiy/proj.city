#include "mesh.h"
#include "Vector/vector.h"

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

namespace urban
{
    namespace shadow
    {
        Mesh::Mesh(void):name("N/A"){}
        Mesh::Mesh(const Mesh & other):name(other.name), points(other.points), faces(other.faces), bounding_box(other.bounding_box){}
        Mesh::Mesh(Mesh && other):name(std::move(other.name)), points(std::move(other.points)), faces(std::move(other.faces)), bounding_box(std::move(other.bounding_box)){}
        Mesh::Mesh(Lib3dsMesh* lib3ds_mesh):name(lib3ds_mesh->name)
        {
            size_t it(0);
            std::for_each(
                lib3ds_mesh->pointL,
                lib3ds_mesh->pointL + lib3ds_mesh->points,
                [&it, this](Lib3dsPoint _point)
                {
                    points.emplace(std::make_pair(it++, Point(_point.pos[0], _point.pos[1], _point.pos[2])));
                }
            );

            it = 0;
            std::for_each(
                lib3ds_mesh->faceL,
                lib3ds_mesh->faceL + lib3ds_mesh->faces,
                [&it, this](Lib3dsFace _face)
                {
                    Point point_0 = points[_face.points[0]];
                    Point point_1 = points[_face.points[1]];
                    Point point_2 = points[_face.points[2]];

                    Vector v1 = Vector(point_0, point_1);
                    Vector v2 = Vector(point_1, point_2);
                    Vector n = Vector(_face.normal[0], _face.normal[1], _face.normal[2]);

                    if(determinant(v1, v2, n)>0)
                        faces.emplace(std::make_pair(it++, Face(_face.points[0], _face.points[1], _face.points[2])));
                    else
                        faces.emplace(std::make_pair(it++, Face(_face.points[0], _face.points[2], _face.points[1])));
                }
            );
            compute_bbox();
        }

        template<class Kernel>
        Mesh::Mesh(const CGAL::Polyhedron_3<Kernel> & polyhedron)
        {
            size_t it(0);
            std::for_each(
                polyhedron.points_begin(),
                polyhedron.points_end(),
                [&it, this](const CGAL::Point_3<Kernel> & point)
                {
                    points.emplace(std::make_pair(it++, Point(point)));
                }
            );

            it = 0;
            std::for_each(
                polyhedron.facets_begin(),
                polyhedron.facets_end(),
                [&it, this](const typename CGAL::Polyhedron_3<Kernel>::Facet & facet)
                {
                    size_t face_degree(facet.facet_degree());
                    std::vector<size_t> face_points(face_degree);
                    face_points[0] = get_index(*(facet.facet_begin()));
                    std::transform(
                        std::next(facet.facet_begin(), 1),
                        std::next(facet.facet_begin(), static_cast<long>(face_degree)),
                        std::next(std::begin(face_points), 1),
                        [&](const typename CGAL::Polyhedron_3<Kernel>::Halfedge & halfedge)
                        {
                            return get_index(halfedge);
                        }
                    );
                    faces.emplace(std::make_pair(it++, Face(face_degree, face_points)));
                }
            );
            compute_bbox();
        }

        Mesh::Mesh(std::string _name, const std::map<size_t, Point> & _points, const std::map<size_t, Face> & _faces):name(_name), points(_points), faces(_faces)
        {
            compute_bbox();
        }

        Mesh::Mesh(std::string _name, const std::map<size_t, Point> & _points, const std::map<size_t, Face> & _faces, const Bbox & _bounding_box):name(_name), points(_points), faces(_faces), bounding_box(_bounding_box){}
        
        Mesh::~Mesh(void){}

        void Mesh::swap(Mesh & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(points, other.points);
            swap(faces, other.faces);
            swap(bounding_box, other.bounding_box);
        }

        Mesh & Mesh::operator=(const Mesh & other) noexcept
        {
            name = other.name;
            points = other.points;
            faces = other.faces;
            bounding_box = bounding_box;
            return *this;
        }

        Mesh & Mesh::operator=(Mesh && other) noexcept
        {
            name = std::move(other.name);
            points = std::move(other.points);
            faces = std::move(other.faces);
            bounding_box = std::move(other.bounding_box);
            return *this;
        }

        std::string Mesh::get_name(void) const noexcept
        {
            return name;
        }

        size_t Mesh::get_number_points(void) const noexcept
        {
            return points.size();
        }

        std::map<size_t, Point> Mesh::get_points(void) const noexcept
        {
            return points;
        }
        
        size_t Mesh::get_number_faces(void) const noexcept
        {
            return faces.size();
        }

        std::map<size_t, Face> Mesh::get_faces(void) const noexcept
        {
            return faces;
        }

        Bbox Mesh::bbox(void) const noexcept
        {
            return bounding_box;
        }

        template<class Kernel>
        size_t Mesh::get_index(const typename CGAL::Polyhedron_3<Kernel>::Halfedge & halfedge)
        {
            size_t index;
            auto point_handle = std::find_if(
                std::begin(points),
                std::end(points),
                [&halfedge](const std::pair<size_t, Point> & p)
                {
                    return p.second == Point(halfedge.vertex()->point());
                }
            );
            if(point_handle != std::end(points))
                index = point_handle->first;
            else
                throw std::out_of_range("The face contains a non listed point");
            return index;
        }


        Lib3dsMesh* Mesh::to_3ds()
        {
            Lib3dsMesh* mesh = reinterpret_cast<Lib3dsMesh*>(calloc(sizeof(Lib3dsMesh), 1));
            strcpy(mesh->name, const_cast<char*>(name.c_str()));
            mesh->points = static_cast<Lib3dsWord>(points.size());
            mesh->pointL = reinterpret_cast<Lib3dsPoint*>(calloc(sizeof(Lib3dsPoint), mesh->points));

            std::transform(
                std::begin(points),
                std::end(points),
                mesh->pointL,
                [&](std::pair<size_t, Point> p)
                {
                    Lib3dsPoint point;
                    auto init = std::initializer_list<double>({p.second.x(), p.second.y(), p.second.z()});
                    std::copy(std::begin(init), std::end(init), point.pos);
                    return point;
                }
            );

            mesh->faces = static_cast<Lib3dsDword>(
                std::accumulate(
                    std::begin(faces), 
                    std::end(faces),
                    0,
                    [](int& size, const std::pair<const size_t, Face> & second)
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
                [this](std::pair<size_t, Face> t)
                {
                    return *t.second.to_3ds(points);
                }
            );

            mesh->next = NULL;

            return mesh;
        }

        std::ostream& operator<<(std::ostream &os, const Mesh & mesh)
        {
            os << "Name: " << mesh.name << std::endl
               << "Bounding box: " << mesh.bounding_box << std::endl
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

        void Mesh::compute_bbox(void)
        {
            bounding_box = std::accumulate(
                std::begin(points),
                std::end(points),
                Bbox(),
                [](Bbox & box, std::pair<size_t, Point> point)
                {
                    return box + point.second.bbox();
                }
            );
        }
    }

    void swap(shadow::Mesh & lhs, shadow::Mesh & rhs)
    {
        lhs.swap(rhs);
    }
}
