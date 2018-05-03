#include <shadow/mesh.h>

#include<scene/unode.h>

#include <stdexcept>

#include <iterator>
#include <algorithm>
#include <numeric>

#include <limits>

namespace city
{
    namespace shadow
    {
        Mesh::Mesh(void)
        {}
        Mesh::Mesh(Mesh const& other)
            : name(other.name), points(other.points), faces(other.faces), bounding_box(other.bounding_box) {}
        Mesh::Mesh(Mesh && other)
            : name(std::move(other.name)), points(std::move(other.points)), faces(std::move(other.faces)), bounding_box(std::move(other.bounding_box)) {}
        Mesh::Mesh(Lib3dsMesh* lib3ds_mesh)
            : name(lib3ds_mesh->name), points(lib3ds_mesh->points), faces(lib3ds_mesh->faces)
        {
            std::transform(
                lib3ds_mesh->pointL,
                lib3ds_mesh->pointL + lib3ds_mesh->points,
                std::begin(points),
                [](Lib3dsPoint const& lib3ds_point)
                {
                    return Point(lib3ds_point);
                }
            );

            std::transform(
                lib3ds_mesh->faceL,
                lib3ds_mesh->faceL + lib3ds_mesh->faces,
                std::begin(faces),
                [this](Lib3dsFace const& lib3ds_facet)
                {
                    Point point_0 = points[lib3ds_facet.points[0]];
                    Point point_1 = points[lib3ds_facet.points[1]];
                    Point point_2 = points[lib3ds_facet.points[2]];
                    Vector n = Vector(
                        static_cast<double>(lib3ds_facet.normal[0]),
                        static_cast<double>(lib3ds_facet.normal[1]),
                        static_cast<double>(lib3ds_facet.normal[2])
                    );

                    return Face(
                        lib3ds_facet.points[0],
                        lib3ds_facet.points[1],
                        lib3ds_facet.points[2],
                        determinant(point_1 - point_0, point_2 - point_1, n) > std::numeric_limits<double>::epsilon()
                    );
                }
            );

            compute_bbox();
        }

        Mesh::Mesh(std::string const& _name, Polyhedron const& polyhedron)
            : name(_name), points(polyhedron.size_of_vertices()), faces(polyhedron.size_of_facets())
        {
            std::transform(
                polyhedron.points_begin(),
                polyhedron.points_end(),
                std::begin(points),
                [](Point_3 const& point)
                {
                    return Point(point);
                }
            );

            std::transform(
                polyhedron.facets_begin(),
                polyhedron.facets_end(),
                std::begin(faces),
                [this](Polyhedron::Facet const& facet)
                {
                    return Face(facet, points);
                }
            );
            compute_bbox();
        }

        Mesh::Mesh(scene::UNode const& unode)
            : Mesh(unode.get_name(), unode.get_surface())
        {}


        Mesh::Mesh(std::string const& _name, std::vector<Point> const& _points, std::vector<Face> const& _faces)
            : name(_name), points(_points), faces(_faces)
        {
            compute_bbox();
        }

        Mesh::Mesh(std::vector<Point> const& _points, std::vector<Face> const& _faces)
            : points(_points), faces(_faces)
        {
            compute_bbox();
        }
        
        Mesh::~Mesh(void)
        {}

        void Mesh::swap(Mesh & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(points, other.points);
            swap(faces, other.faces);
            swap(bounding_box, other.bounding_box);
        }

        Mesh & Mesh::operator =(Mesh const& other) noexcept
        {
            name = other.name;
            points = other.points;
            faces = other.faces;
            bounding_box = bounding_box;
            return *this;
        }
        Mesh & Mesh::operator =(Mesh && other) noexcept
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
        Mesh::points_const_iterator Mesh::points_begin(void) const noexcept
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
        Mesh::points_const_iterator Mesh::points_end(void) const noexcept
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
        bool Mesh::is_empty(void) const noexcept
        {
            return points.empty();
        }


        Mesh Mesh::set_name(std::string const& _name) noexcept
        {
            name = _name;
            return *this;
        }

        std::string Mesh::get_name(void) const noexcept
        {
            return name;
        }
        std::size_t Mesh::points_size(void) const noexcept
        {
            return points.size();
        }
        std::vector<Point> const& Mesh::get_points(void) const noexcept
        {
            return points;
        }
        std::vector<Point> & Mesh::get_points(void) noexcept
        {
            return points;
        }
        std::vector<Point_3> Mesh::get_cgal_points(void) const noexcept
        {
            std::vector<Point_3> cgal_points(points.size());
            std::transform(
                std::begin(points),
                std::end(points),
                std::begin(cgal_points),
                [](Point const& point)
                {
                    return Point_3(point.x(), point.y(), point.z());
                }
            );
            return cgal_points;
        }
        std::size_t Mesh::faces_size(void) const noexcept
        {
            return faces.size();
        }
        std::vector<Face> const& Mesh::get_faces(void) const noexcept
        {
            return faces;
        }
        std::vector<Face> & Mesh::get_faces(void) noexcept
        {
            return faces;
        }
        std::vector< std::vector<std::size_t> > Mesh::get_cgal_faces(void) const noexcept
        {
            std::vector< std::vector<std::size_t> > cgal_faces(faces.size());
            std::transform(
                std::begin(faces),
                std::end(faces),
                std::begin(cgal_faces),
                [](Face const& face)
                {
                    return face.indexes();
                }
            );

            return cgal_faces;
        }
        Bbox const& Mesh::bbox(void) const noexcept
        {
            return bounding_box;
        }

        Mesh & Mesh::operator +=(Mesh const& other)
        {
            if(is_empty())
                *this = other;
            else
            {
                name += "_" + other.name;
                bounding_box += other.bounding_box;

                auto diff = points.size();
                auto shift = faces.size();
                
                points.insert(std::end(points), std::begin(other.points), std::end(other.points));
                faces.insert(std::end(faces), std::begin(other.faces), std::end(other.faces));

                std::transform(
                    std::next(std::begin(faces), static_cast<long>(shift)),
                    std::end(faces),
                    std::next(std::begin(faces), static_cast<long>(shift)),
                    [diff](Face & face)
                    {
                        return face.offset(diff);
                    }
                );
            }
            return *this;
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
                [](Point const& point)
                {
                    Lib3dsPoint lib3ds_point;
                    auto init = std::initializer_list<double>({point.x(), point.y(), point.z()});
                    std::copy(std::begin(init), std::end(init), lib3ds_point.pos);
                    return lib3ds_point;
                }
            );

            mesh->faces = static_cast<Lib3dsDword>(
                std::accumulate(
                    std::begin(faces), 
                    std::end(faces),
                    0,
                    [](int & size, Face const& face)
                        {
                            return size + static_cast<int>(face.degree()) - 2;
                        }
                    )
                );

            mesh->faceL = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace), mesh->faces));

            std::transform(
                std::begin(faces),
                std::end(faces),
                mesh->faceL,
                [this](Face const& face)
                {
                    return *(face.to_3ds(points));
                }
            );

            mesh->next = nullptr;

            return mesh;
        }

        std::ostream& operator <<(std::ostream &os, Mesh const& mesh)
        {
            os << "#Name: " << mesh.name << std::endl
               << "#Bounding box: " << mesh.bounding_box << std::endl
               << "OFF" << std::endl;

            os << mesh.points_size() << " " << mesh.faces_size() << " 0" << std::endl;
            for(std::size_t idx = 0; idx < mesh.points_size(); ++idx)
                os << mesh.points.at(idx) << std::endl;

            for(std::size_t idx = 0; idx < mesh.faces_size(); ++idx)
                os << mesh.faces.at(idx) << std::endl;

            return os;
        }

        void Mesh::compute_bbox(void)
        {
            bounding_box = std::accumulate(
                std::begin(points),
                std::end(points),
                Bbox(),
                [](Bbox & box, Point const& point)
                {
                    return box + point.bbox();
                }
            );
        }

        Mesh operator +(Mesh const& lhs, Mesh const& rhs)
        {
            return Mesh(lhs) += rhs;
        }

        bool operator ==(Mesh const& lhs, Mesh const& rhs)
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
                    std::equal_to<Point>()
                );
                if(equal)
                {
                    equal = std::inner_product(
                        lhs.faces_cbegin(),
                        lhs.faces_cend(),
                        rhs.faces_cbegin(),
                        true,
                        std::logical_and<bool>(),
                        std::equal_to<Face>()
                    );
                }
            }
            return equal;
        }

        bool operator !=(Mesh const& lhs, Mesh const& rhs)
        {
            return !(lhs == rhs);
        }        
    }

    void swap(shadow::Mesh & lhs, shadow::Mesh & rhs)
    {
        lhs.swap(rhs);
    }
}
