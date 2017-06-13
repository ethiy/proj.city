#include "unode.h"

namespace urban
{
    namespace scene
    {
        UNode::UNode(void) 
        {}
        UNode::UNode(UNode const& other)
            :name(std::move(other.name)), reference_point(other.reference_point), epsg_index(other.epsg_index), surface(other.surface), bounding_box(other.bounding_box)
        {}
        UNode::UNode(UNode && other)
            :name(std::move(other.name)), reference_point(std::move(other.reference_point)), epsg_index(std::move(other.epsg_index)), surface(std::move(other.surface)), bounding_box(std::move(other.bounding_box))
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
                [](shadow::Face const& face)
                {
                    std::vector<std::size_t> buffer(face.size());
                    std::copy(std::begin(face), std::end(face), std::begin(buffer));
                    return buffer;
                }
            );
        }
        UNode::~UNode(void)
        {}
    }
}
