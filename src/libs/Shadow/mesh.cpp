#include "mesh.h"
#include "Vector/vector.h"

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <limits>

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
                    points.emplace(std::make_pair(it++, Point(static_cast<double>(_point.pos[0]), static_cast<double>(_point.pos[1]), static_cast<double>(_point.pos[2]))));
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
                    Vector n = Vector(static_cast<double>(_face.normal[0]), static_cast<double>(_face.normal[1]), static_cast<double>(_face.normal[2]));

                    if(determinant(v1, v2, n) > std::numeric_limits<float>::epsilon())
                        faces.emplace(std::make_pair(it++, Face(_face.points[0], _face.points[1], _face.points[2])));
                    else
                        faces.emplace(std::make_pair(it++, Face(_face.points[0], _face.points[2], _face.points[1])));
                }
            );
            compute_bbox();
        }

        Mesh::Mesh(const Polyhedron & polyhedron)
        {
            size_t it(0);
            std::for_each(
                polyhedron.points_begin(),
                polyhedron.points_end(),
                [&it, this](const Point_3 & point)
                {
                    points.emplace(std::make_pair(it++, Point(point)));
                }
            );

            it = 0;
            std::for_each(
                polyhedron.facets_begin(),
                polyhedron.facets_end(),
                [&it, this](const Facet & facet)
                {
                    size_t face_degree(facet.facet_degree());
                    std::vector<size_t> face_points(face_degree);
                    face_points[0] = get_index(*(facet.facet_begin()));
                    std::transform(
                        std::next(facet.facet_begin(), 1),
                        std::next(facet.facet_begin(), static_cast<long>(face_degree)),
                        std::next(std::begin(face_points), 1),
                        [&](const Polyhedron::Halfedge & halfedge)
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

        Mesh::points_iterator Mesh::points_begin(void) noexcept
        {
            return std::begin(points);
        }
            
        Mesh::points_const_iterator Mesh::points_cbegin(void) const noexcept
        {
            return points.cbegin();
        }
            
        Mesh::points_iterator Mesh::points_end(void) noexcept
        {
            return std::end(points);
        }
            
        Mesh::points_const_iterator Mesh::points_cend(void) const noexcept
        {
            return points.cend();
        }
            
        Mesh::faces_iterator Mesh::faces_begin(void) noexcept
        {
            return std::begin(faces);
        }
            
        Mesh::faces_const_iterator Mesh::faces_cbegin(void) const noexcept
        {
            return faces.cbegin();
        }
            
        Mesh::faces_iterator Mesh::faces_end(void) noexcept
        {
            return std::end(faces);
        }
            
        Mesh::faces_const_iterator Mesh::faces_cend(void) const noexcept
        {
            return faces.cend();
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

        size_t Mesh::get_index(const Polyhedron::Halfedge & halfedge)
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

        bool operator==(const shadow::Mesh & lhs, const shadow::Mesh & rhs)
        {
            bool equal(false);
            if(lhs.get_number_points() == rhs.get_number_points() && lhs.get_number_faces() == rhs.get_number_faces())
            {
                std::vector<bool> points_equalities(lhs.get_number_points());
                std::vector<bool> faces_equalities(lhs.get_number_faces());
                std::transform(
                    lhs.points_cbegin(),
                    lhs.points_cend(),
                    rhs.points_cbegin(),
                    std::begin(points_equalities),
                    [](const std::pair<size_t, Point> & l_point_pair, const std::pair<size_t, Point> & r_point_pair)
                    {
                        return l_point_pair.second == r_point_pair.second;
                    }
                );

                equal = std::accumulate(
                    std::begin(points_equalities),
                    std::end(points_equalities),
                    true,
                    [](bool & result, const bool & point_equality)
                    {
                        return result && point_equality;
                    }
                );

                if(equal)
                {
                    std::transform(
                        lhs.faces_cbegin(),
                        lhs.faces_cend(),
                        rhs.faces_cbegin(),
                        std::begin(faces_equalities),
                        [](const std::pair<size_t, Face> & l_face_pair, const std::pair<size_t, Face> & r_face_pair)
                        {
                            return l_face_pair.second == r_face_pair.second;
                        }
                    );

                    equal &= std::accumulate(
                        std::begin(faces_equalities),
                        std::end(faces_equalities),
                        true,
                        [](bool & result, const bool & face_equality)
                        {
                            return result && face_equality;
                        }
                    );
                }
            }
            return equal;
        }

        bool operator!=(const shadow::Mesh & lhs, const shadow::Mesh & rhs)
        {
            return !(lhs == rhs);
        }        
    }

    void swap(shadow::Mesh & lhs, shadow::Mesh & rhs)
    {
        lhs.swap(rhs);
    }
}
