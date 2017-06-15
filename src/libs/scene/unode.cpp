#include "unode.h"

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>

namespace urban
{
    namespace scene
    {
        UNode::UNode(void) 
        {}
        UNode::UNode(UNode const& other)
            :name(std::move(other.name)),
             reference_point(other.reference_point),
             epsg_index(other.epsg_index),
             surface(other.surface),
             bounding_box(other.bounding_box)
        {}
        UNode::UNode(UNode && other)
            :name(std::move(other.name)),
             reference_point(std::move(other.reference_point)),
             epsg_index(std::move(other.epsg_index)),
             surface(std::move(other.surface)),
             bounding_box(std::move(other.bounding_box))
        {}
        UNode::UNode(std::string const& building_id, io::FileHandler<Lib3dsFile> const& mesh_file)
            :name(building_id)
        {
            std::vector<shadow::Mesh> meshes = mesh_file.read(building_id);
            std::size_t point_size = std::accumulate(
                std::begin(meshes),
                std::end(meshes),
                0,
                [](std::size_t _size, shadow::Mesh const& mesh)
                {
                    return _size + mesh.points_size();
                }
            );

            std::vector<Point_3> points(point_size);
            std::vector<Point_3> buffer;
            for(auto const& mesh : meshes)
            {
                buffer = std::vector<Point_3>(mesh.points_size());
                std::transform(
                    mesh.points_cbegin(),
                    mesh.points_cend(),
                    std::begin(buffer),
                    [](std::pair<std::size_t, shadow::Point> const& point_p)
                    {
                        return Point_3(point_p.second.x(), point_p.second.y(), point_p.second.z());
                    }
                );
                points.insert(std::end(points), std::begin(buffer), std::end(buffer));
            }

            std::vector< std::vector<std::size_t> > polygons(meshes.size());
            std::transform(
                std::begin(meshes),
                std::end(meshes),
                std::begin(polygons),
                [](shadow::Mesh const& mesh)
                {
                    std::vector<std::size_t> buffer(face.get_degree());
                    std::copy(std::begin(face), std::end(face), std::begin(buffer));
                    return buffer;
                }
            );

            CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface);
            if (CGAL::is_closed(surface) && (!CGAL::Polygon_mesh_processing::is_outward_oriented(surface)))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            CGAL::Polygon_mesh_processing::stitch_borders(surface);
        }
        UNode::~UNode(void)
        {}
        
        void UNode::swap(UNode & other)
        {
            using std::swap;

            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(epsg_index, other.epsg_index);
            swap(surface, other.surface);
            swap(bounding_box, other.bounding_box);
        }
        UNode & UNode::operator =(UNode const& other) noexcept
        {
            name = other.name;
            reference_point = other.reference_point;
            epsg_index = other.epsg_index;
            surface = other.surface;
            bounding_box = other.bounding_box;

            return *this;
        }
        UNode & UNode::operator =(UNode && other) noexcept
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            epsg_index = std::move(other.epsg_index);
            surface = std::move(other.surface);
            bounding_box = std::move(other.bounding_box);

            return *this;
        }
        
        void swap(UNode & lhs, UNode & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
