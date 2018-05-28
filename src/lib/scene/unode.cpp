#include <scene/unode.h>

#include <algorithms/util_algorithms.h>
#include <scene/utilities.h>

#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>

#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#endif // CGAL_USE_GEOMVIEW

#include <CGAL/aff_transformation_tags.h>
#include <CGAL/squared_distance_3.h>

#include <stdexcept>

#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>

#include <cmath>


namespace city
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
        UNode::UNode(
            shadow::Mesh const& mesh,
            Point_3 const& _reference_point,
            unsigned short const _epsg_index
        )
            : UNode(mesh.get_name(), mesh.get_cgal_points(), mesh.get_cgal_faces(), _reference_point, _epsg_index)
        {}
        UNode::UNode(
            std::string const& building_id,
            std::vector<Point_3> points,
            std::vector< std::vector<std::size_t> > polygons,
            Point_3 const& _reference_point,
            unsigned short const _epsg_index
        )
            : name(building_id),
              reference_point(_reference_point),
              epsg_index(_epsg_index),
              surface(
                    polyhedron_from_polygon_soup(
                        points,
                        polygons
                    )
              )
        {
            if(!surface.empty())
                bounding_box = CGAL::Polygon_mesh_processing::bbox(surface);
            set_face_ids();
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

        Bbox_3 const& UNode::bbox(void) const noexcept
        {
            return bounding_box;
        }

        std::string const& UNode::get_name(void) const noexcept
        {
            return name;
        }

        unsigned short const& UNode::get_epsg(void) const noexcept
        {
            return epsg_index;
        }

        Point_3 const& UNode::get_reference_point(void) const noexcept
        {
            return reference_point;
        }

        Polyhedron const& UNode::get_surface(void) const noexcept
        {
            return surface;
        }

        std::size_t UNode::vertices_size(void) const
        {
            return surface.size_of_vertices();
        }

        std::size_t UNode::facets_size(void) const
        {
            return surface.size_of_facets();
        }

        UNode::Facet_iterator UNode::facets_begin(void) noexcept
        {
            return surface.facets_begin();
        }
        UNode::Facet_iterator UNode::facets_end(void) noexcept
        {
            return surface.facets_end();
        }
        UNode::Facet_const_iterator UNode::facets_begin(void) const noexcept
        {
            return surface.facets_begin();
        }
        UNode::Facet_const_iterator UNode::facets_end(void) const noexcept
        {
            return surface.facets_end();
        }
        UNode::Facet_const_iterator UNode::facets_cbegin(void) const noexcept
        {
            return surface.facets_begin();
        }
        UNode::Facet_const_iterator UNode::facets_cend(void) const noexcept
        {
            return surface.facets_end();
        }

        UNode::Halfedge_iterator UNode::halfedges_begin(void) noexcept
        {
            return surface.halfedges_begin();
        }
        UNode::Halfedge_iterator UNode::halfedges_end(void) noexcept
        {
            return surface.halfedges_end();
        }
        UNode::Halfedge_const_iterator UNode::halfedges_begin(void) const noexcept
        {
            return surface.halfedges_begin();
        }
        UNode::Halfedge_const_iterator UNode::halfedges_end(void) const noexcept
        {
            return surface.halfedges_end();
        }
        UNode::Halfedge_const_iterator UNode::halfedges_cbegin(void) const noexcept
        {
            return surface.halfedges_begin();
        }
        UNode::Halfedge_const_iterator UNode::halfedges_cend(void) const noexcept
        {
            return surface.halfedges_end();
        }

        UNode::Halfedge_iterator UNode::border_halfedges_begin(void) noexcept
        {
            return surface.border_halfedges_begin();
        }

        UNode::Halfedge_const_iterator UNode::border_halfedges_begin(void) const noexcept
        {
            return surface.border_halfedges_begin();
        }


        UNode::Point_iterator UNode::points_begin(void) noexcept
        {
            return surface.points_begin();
        }
        UNode::Point_iterator UNode::points_end(void) noexcept
        {
            return surface.points_end();
        }
        UNode::Point_const_iterator UNode::points_cbegin(void) const noexcept
        {
            return surface.points_begin();
        }
        UNode::Point_const_iterator UNode::points_cend(void) const noexcept
        {
            return surface.points_end();
        }

        UNode::Plane_iterator UNode::planes_begin(void) noexcept
        {
            return surface.planes_begin();
        }
        UNode::Plane_iterator UNode::planes_end(void) noexcept
        {
            return surface.planes_end();
        }
        UNode::Plane_const_iterator UNode::planes_cbegin(void) const noexcept
        {
            return surface.planes_begin();
        }
        UNode::Plane_const_iterator UNode::planes_cend(void) const noexcept
        {
            return surface.planes_end();
        }

        bool UNode::empty(void) const
        {
            return surface.empty();
        }

        UNode::Halfedge_iterator UNode::prunable(void)
        {
            return std::find_if(
                halfedges_begin(),
                halfedges_end(),
                [this](Polyhedron::Halfedge & halfedge)
                {
                    return  !halfedge.is_border_edge()
                            &&
                            coplanar(surface, halfedge.facet(), halfedge.opposite()->facet())
                            &&
                            !do_open_coplanar_intersect(surface, halfedge.facet(), halfedge.opposite()->facet())
                            &&
                            is_union_coplanar_simple(surface, halfedge.facet(), halfedge.opposite()->facet());
                }
            );
        }
        UNode& UNode::prune(void)
        {
            auto halfedge_handle = prunable();

            while(halfedge_handle != halfedges_end())
            {
                join_facet(halfedge_handle);
                halfedge_handle = prunable();
            }
            stitch_borders();

            return *this;
        }
        UNode & UNode::join_facet(UNode::Halfedge_handle & h)
        {
            surface.join_facet(h);
            return *this;
        }
        UNode & UNode::stitch_borders(void)
        {
            CGAL::Polygon_mesh_processing::stitch_borders(surface);
            return *this;
        }

        Point_3 UNode::centroid(UNode::Facet_const_handle facet) const
        {
            auto circulator = facet->facet_begin();
            Vector_3 n = normal(facet);

            Vector_3 centroid = CGAL::NULL_VECTOR;
            do
            {
                centroid =  centroid
                            +
                            ((circulator->vertex()->point() - CGAL::ORIGIN) + (circulator->next()->vertex()->point() - CGAL::ORIGIN))
                                *
                            to_double(CGAL::cross_product(circulator->vertex()->point() - CGAL::ORIGIN, circulator->next()->vertex()->point() - CGAL::ORIGIN) * n)
                                /
                            6;
            }while(++circulator != facet->facet_begin());

            return CGAL::ORIGIN + centroid / area(facet);
        }
        Vector_3 UNode::normal(UNode::Facet_const_handle facet) const
        {
            UNode::Facet _face = *facet;
            return CGAL::Polygon_mesh_processing::compute_face_normal(&_face, surface);
        }
        double UNode::area(UNode::Facet_const_handle facet) const
        {
            ExactToInexact to_inexact;
            UNode::Facet _face = *facet;
            return to_inexact(CGAL::Polygon_mesh_processing::face_area(&_face, surface));
        }
        double UNode::circumference(UNode::Facet_const_handle facet) const
        {
            auto circulator = facet->facet_begin();
            double result = 0;
            
            do
            {
                result += std::sqrt(
                    to_double(
                        CGAL::squared_distance(
                            circulator->vertex()->point(),
                            circulator->opposite()->vertex()->point()
                        )
                    )
                );
            }while(++circulator != facet->facet_begin());

            return result;
        }
        
        UNode & UNode::set_face_ids(void)
        {
            size_t index(0);
            auto iterator = surface.facets_begin();

            for(; iterator != surface.facets_end(); ++iterator)
                iterator->id() = index ++;
            
            return *this;
        }
        
        std::vector<UNode::Facet_const_handle> UNode::facet_adjacents(UNode::Facet const& facet) const
        {
            std::vector<UNode::Facet_const_handle> adjacents;
            adjacents.reserve(facet.facet_degree());

            auto circulator = facet.facet_begin();
            do
            {
                auto buff = circulator->opposite()->facet();
                if(!circulator->is_border() && buff != nullptr)
                    adjacents.push_back(buff);
            }while(++circulator != facet.facet_begin());
            return adjacents;
        }
        std::vector<UNode::Facet_const_handle> UNode::facet_handles(void) const
        {
            std::vector<UNode::Facet_const_handle> facets(facets_size());
            std::transform(
                facets_cbegin(),
                facets_cend(),
                std::begin(facets),
                [](UNode::Facet const& facet)
                {
                    return &facet;
                }
            );
            return facets;
        }
        std::vector<bool> UNode::facet_adjacency_matrix(void) const
        {
            std::vector<bool> matrix(facets_size() * facets_size(), false);

            for(std::size_t diag(0); diag != facets_size(); ++diag)
                matrix.at(diag * facets_size() + diag) = true;

            auto facets = facet_handles();

            std::vector<UNode::Facet_const_handle> line_adjacents;
            for(std::size_t line(0); line != facets_size(); ++line)
            {
                line_adjacents = facet_adjacents(*facets.at(line));

                for(auto adjacent : line_adjacents)
                {
                    auto placeholder = std::find(std::begin(facets), std::end(facets), adjacent);
                    if(placeholder != std::end(facets))
                        matrix.at(
                            line * facets_size()
                            +
                            static_cast<std::size_t>(std::distance(std::begin(facets), placeholder))
                        )
                        =
                        true;
                }
            }   
            return matrix;
        }

        UNode& UNode::transform(const Affine_transformation_3 & affine_transformation)
        {
            std::transform(
                points_begin(),
                points_end(),
                points_begin(),
                [& affine_transformation](Point_3 & point)
                {
                    return affine_transformation.transform(point);
                }
            );
            reference_point = affine_transformation.transform(reference_point);
            return *this;
        }

        std::ostream & operator <<(std::ostream &os, UNode const& unode)
        {
            os  << "# Name: " << unode.name << std::endl
                << unode.surface;
            return os;
        }

        io::Adjacency_stream & operator <<(io::Adjacency_stream & as, UNode const& unode)
        {
            std::for_each(
                unode.facets_cbegin(),
                unode.facets_cend(),
                [&as, &unode](UNode::Facet const& facet)
                {
                    as << facet.id() << " " << facet.facet_degree() << " " << unode.area(facet.halfedge()->facet()) << " " << unode.circumference(facet.halfedge()->facet()) << " " << unode.centroid(facet.halfedge()->facet()) << " " << unode.normal(facet.halfedge()->facet()) << std::endl;
                }
            );

            std::vector<bool> matrix = unode.facet_adjacency_matrix();

            as << matrix << std::endl;

            return as;
        }

        #ifdef CGAL_USE_GEOMVIEW
        CGAL::Geomview_stream & operator <<(CGAL::Geomview_stream &gs, UNode const& unode)
        {
            gs << unode.surface;
            return gs;
        }
        #endif // CGAL_USE_GEOMVIEW

        void swap(UNode & lhs, UNode & rhs)
        {
            lhs.swap(rhs);
        }

        double border_length(UNode const& unode)
        {
            return std::accumulate(
                unode.border_halfedges_begin(),
                unode.halfedges_end(),
                .0,
                [](double & length, const Polyhedron::Halfedge & halfedge)
                {
                    return length + std::sqrt(to_double(Vector_3(halfedge.next()->vertex()->point(), halfedge.vertex()->point()) * Vector_3(halfedge.next()->vertex()->point(), halfedge.vertex()->point())));
                }
            );
        }

        UNode& affine_transform(UNode & unode, const Affine_transformation_3 & affine_transformation)
        {
            unode.transform(affine_transformation);
            return unode;
        }

        UNode& translate(UNode & unode, const Vector_3 & offset)
        {
            Affine_transformation_3 translation(CGAL::TRANSLATION, offset);
            return affine_transform(unode, translation);
        }

        UNode& scale(UNode & unode, double scale)
        {
            Affine_transformation_3 scaling(CGAL::SCALING, scale);
            return affine_transform(unode, scaling);
        }

        UNode& rotate(UNode & unode, const Vector_3 & axis, double angle)
        {
            std::map<double, Vector_3> _rotation{{angle, axis}};
            Affine_transformation_3 rotation(rotation_transform(_rotation));
            return affine_transform(unode, rotation);
        }

        UNode& rotate(UNode & unode, const std::map<double, Vector_3> & _rotations)
        {
            Affine_transformation_3 rotation(rotation_transform(_rotations));
            return affine_transform(unode, rotation);
        }

        UNode& prune(UNode & unode)
        {
            return unode.prune();
        }

        double area(UNode const& unode)
        {
            return std::accumulate(
                unode.facets_begin(),
                unode.facets_end(),
                .0,
                [&unode](double area, UNode::Facet const& facet)
                {
                    return area + unode.area(facet.halfedge()->facet());
                }
            );
        }

        double total_edge_length(UNode const& unode)
        {
            return std::accumulate(
                unode.halfedges_begin(),
                unode.halfedges_end(),
                .0,
                [](double total_length, Polyhedron::Halfedge const& halfedge)
                {
                    return  total_length
                            +
                            std::sqrt(
                                to_double(
                                    CGAL::squared_distance(
                                        halfedge.vertex()->point(),
                                        halfedge.opposite()->vertex()->point()
                                    )
                                )
                            );
                }
            );
        }
    }
}
