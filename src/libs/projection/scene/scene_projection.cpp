#include "scene_projection.h"

namespace urban
{
    namespace projection
    {
        FootPrint::FootPrint(UNode const& unode)
            : name(unode.get_name()), reference_point(unode.get_reference_point()), epsg_index(unode.get_epsg_index())
        {
            std::vector<BrickPrint> prints(unode.facets_size());
            std::vector<Point_2> buffer_points;

            std::transform(
                unode.facets_cbegin(),
                unode.facets_cend(),
                std::begin(prints),
                [&buffer_points](scene::UNode::Facet const& facet)
                {
                    buffer_points = std::vector<Point_2>(facet.facet_degree());

                    auto halfedge = facet.halfedge();
                    Point_3 vertex(halfedge->vertex()->point());
                    buffer_points.at(0) = Point_2(vertex.x(), vertex.y());
                    std::transform(
                        std::next(facet.facet_begin()),
                        std::next(facet.facet_begin(), static_cast<long>(facet.facet_degree())),
                        std::next(std::begin(buffer_points)),
                        [](Polyhedron::Halfedge const& h)
                        {
                            return Point_2(h.vertex()->point().x(), h.vertex()->point().y());
                        }
                    );

                    
                projection::FacePrint projected_facet;
                    return BrickPrint(projected_facet);
                }
            );
        }
        FootPrint::FootPrint(FootPrint const& other)
            : name(other.name), reference_point(other.reference_point), epsg_index(other.epsg_index), projection(other.projection)
        {}
        FootPrint::FootPrint(FootPrint && other)
            : name(std::move(other.name)), reference_point(std::move(other.reference_point)), epsg_index(std::move(other.epsg_index)), projection(std::move(other.projection))
        {}
        FootPrint::~FootPrint(void)
        {}

        void FootPrint::swap(FootPrint & other)
        {
            using std::swap;

            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(epsg_index, other.epsg_index);
            swap(projection, other.projection);
        }
        FootPrint & FootPrint::operator =(FootPrint const& other)
        {
            name = other.name;
            reference_point = other.reference_point;
            epsg_index = other.epsg_index;
            projection = other.projection;

            return *this;
        }
        FootPrint & FootPrint::operator =(FootPrint && other)
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            epsg_index = std::move(other.epsg_index);
            projection = std::move(other.projection);

            return *this;
        }

        void FootPrint::swap(FootPrint & lhs, FootPrint & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
