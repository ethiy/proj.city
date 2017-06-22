#include "mesh.h"
#include "Geometry/vector.h"

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <limits>

namespace urban
{
    namespace shadow
    {
        Mesh::Mesh(void)
            : name("no name") {}
        Mesh::Mesh(Mesh const& other)
            : name(other.name), points(other.points), faces(other.faces), bounding_box(other.bounding_box) {}
        Mesh::Mesh(Mesh && other)
            : name(std::move(other.name)), points(std::move(other.points)), faces(std::move(other.faces)), bounding_box(std::move(other.bounding_box)) {}
        Mesh::Mesh(Lib3dsMesh* lib3ds_mesh)
            : name(lib3ds_mesh->name)
        {
            std::size_t it(0);
            std::for_each(
                lib3ds_mesh->pointL,
                lib3ds_mesh->pointL + lib3ds_mesh->points,
                [&it, this](Lib3dsPoint const& lib3ds_point)
                {
                    points.emplace(std::make_pair(it++, Point(lib3ds_point)));
                }
            );

            it = 0;
            std::for_each(
                lib3ds_mesh->faceL,
                lib3ds_mesh->faceL + lib3ds_mesh->faces,
                [&it, this](Lib3dsFace const& lib3ds_facet)
                {
                    Point point_0 = points[lib3ds_facet.points[0]];
                    Point point_1 = points[lib3ds_facet.points[1]];
                    Point point_2 = points[lib3ds_facet.points[2]];
                    Vector n = Vector(
                        static_cast<double>(lib3ds_facet.normal[0]),
                        static_cast<double>(lib3ds_facet.normal[1]),
                        static_cast<double>(lib3ds_facet.normal[2])
                    );

                    faces.emplace(
                        std::make_pair(
                            it++,
                            Face(
                                lib3ds_facet.points[0],
                                lib3ds_facet.points[1],
                                lib3ds_facet.points[2],
                                determinant(point_1 - point_0, point_2 - point_1, n) > std::numeric_limits<double>::epsilon()
                            )
                        )
                    );
                }
            );
            compute_bbox();
        }

        Mesh::Mesh(Polyhedron const& polyhedron)
        {
            std::size_t it(0);
            std::for_each(
                polyhedron.points_begin(),
                polyhedron.points_end(),
                [&it, this](Point_3 const& point)
                {
                    points.emplace(std::make_pair(it++, Point(point)));
                }
            );

            it = 0;
            std::for_each(
                polyhedron.facets_begin(),
                polyhedron.facets_end(),
                [&it, this](Polyhedron::Facet const& facet)
                {
                    std::size_t face_degree(facet.facet_degree());
                    std::vector<std::size_t> face_points(face_degree);
                    face_points[0] = get_index(*(facet.facet_begin()));
                    std::transform(
                        std::next(facet.facet_begin(), 1),
                        std::next(facet.facet_begin(), static_cast<long>(face_degree)),
                        std::next(std::begin(face_points), 1),
                        [this](Polyhedron::Halfedge const& halfedge)
                        {
                            return get_index(halfedge);
                        }
                    );
                    faces.emplace(std::make_pair(it++, Face(face_points)));
                }
            );
            compute_bbox();
        }

        Mesh::Mesh(std::string _name, std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const& _faces)
            : name(_name), points(_points), faces(_faces)
        {
            compute_bbox();
        }

        Mesh::Mesh(std::map<std::size_t, Point> const& _points, std::map<std::size_t, Face> const& _faces)
            : points(_points), faces(_faces)
        {
            compute_bbox();
        }
        
        Mesh::~Mesh(void) {}

        void Mesh::swap(Mesh & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(points, other.points);
            swap(faces, other.faces);
            swap(bounding_box, other.bounding_box);
        }

        Mesh & Mesh::operator=(Mesh const& other) noexcept
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


        void Mesh::set_name(std::string const& _name) noexcept
        {
            name = _name;
        }

        std::string Mesh::get_name(void) const noexcept
        {
            return name;
        }
        std::size_t Mesh::points_size(void) const noexcept
        {
            return points.size();
        }
        std::map<std::size_t, Point> Mesh::get_points(void) const noexcept
        {
            return points;
        }
        std::size_t Mesh::faces_size(void) const noexcept
        {
            return faces.size();
        }
        std::map<std::size_t, Face> Mesh::get_faces(void) const noexcept
        {
            return faces;
        }
        Bbox Mesh::bbox(void) const noexcept
        {
            return bounding_box;
        }

        Lib3dsMesh* Mesh::to_3ds(void) const
        {
            char name_buffer[64];
            strncpy(name_buffer, name.c_str(), 63);

            Lib3dsMesh* mesh = lib3ds_mesh_new(name_buffer);
            mesh->points = static_cast<Lib3dsWord>(points.size());
            lib3ds_mesh_new_point_list(mesh, mesh->points);

            std::transform(
                std::begin(points),
                std::end(points),
                mesh->pointL,
                [](std::pair<std::size_t, Point> const& p)
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
                    [](int & size, std::pair<std::size_t, Face> const& f)
                        {
                            return size + static_cast<int>(f.second.degree()) - 2;
                        }
                    )
                );

            mesh->faceL = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), mesh->faces));

            std::transform(
                std::begin(faces),
                std::end(faces),
                mesh->faceL,
                [this](std::pair<std::size_t, Face> const& t)
                {
                    return *t.second.to_3ds(points);
                }
            );

            mesh->next = NULL;

            return mesh;
        }

        std::ostream& operator<<(std::ostream &os, Mesh const& mesh)
        {
            os << "Name: " << mesh.name << std::endl
               << "Bounding box: " << mesh.bounding_box << std::endl
               << "Points: " << std::endl;

            std::for_each(
                std::begin(mesh.points),
                std::end(mesh.points),
                [&os](std::pair<std::size_t, Point> const& p)
                {
                    os << "Point " << p.first << " : " << p.second << std::endl;
                }
            );

            os << "Faces: " << std::endl;

            std::for_each(
                std::begin(mesh.faces),
                std::end(mesh.faces),
                [&os](std::pair<std::size_t, Face> const& t)
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
                [](Bbox & box, std::pair<std::size_t, Point> const& point)
                {
                    return box + point.second.bbox();
                }
            );
        }

        std::size_t Mesh::get_index(Polyhedron::Halfedge const& halfedge)
        {
            std::size_t index(0);
            auto point_handle = std::find_if(
                std::begin(points),
                std::end(points),
                [&halfedge](std::pair<std::size_t, Point> const& p)
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

        bool operator==(shadow::Mesh const& lhs, shadow::Mesh const& rhs)
        {
            bool equal(lhs.points_size() == rhs.points_size() && lhs.faces_size() == rhs.faces_size());
            if(equal)
            {
                equal = std::inner_product(
                    lhs.points_cbegin(),
                    lhs.points_cend(),
                    rhs.points_cbegin(),
                    true,
                    std::logical_and<bool>(),
                    [](std::pair<std::size_t, Point> const& l_point_pair, std::pair<std::size_t, Point> const& r_point_pair)
                    {
                        return l_point_pair.second == r_point_pair.second;
                    }
                );
                if(equal)
                {
                    equal = std::inner_product(
                        lhs.faces_cbegin(),
                        lhs.faces_cend(),
                        rhs.faces_cbegin(),
                        true,
                        std::logical_and<bool>(),
                        [](std::pair<std::size_t, Face> const& l_face_pair, std::pair<std::size_t, Face> const& r_face_pair)
                        {
                            return l_face_pair.second == r_face_pair.second;
                        }
                    );
                }
            }
            return equal;
        }

        bool operator!=(shadow::Mesh const& lhs, shadow::Mesh const& rhs)
        {
            return !(lhs == rhs);
        }        
    }

    void swap(shadow::Mesh & lhs, shadow::Mesh & rhs)
    {
        lhs.swap(rhs);
    }
}
