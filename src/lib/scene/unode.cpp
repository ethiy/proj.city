#include <scene/unode.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/measure.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#endif // CGAL_USE_GEOMVIEW

#include <vector>


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
        UNode::UNode(std::string const& building_id, shadow::Point const& _reference_point, unsigned short const _epsg_index, io::FileHandler<Lib3dsFile> const& mesh_file)
            : UNode(building_id, _reference_point, _epsg_index, mesh_file.read_and_stitch(building_id))
        {}
        UNode::UNode(std::string const& building_id, shadow::Point const& _reference_point, unsigned short const _epsg_index, shadow::Mesh const& mesh)
            :name(building_id), reference_point(_reference_point), epsg_index(_epsg_index)
        {
            std::vector<Point_3> points = mesh.get_cgal_points();
            std::vector< std::vector<std::size_t> > polygons = mesh.get_cgal_faces();

            CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface);
            CGAL::Polygon_mesh_processing::stitch_borders(surface);
            if (!CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            if(!surface.empty())
                bounding_box = CGAL::Polygon_mesh_processing::bbox(surface);
        }
        UNode::UNode(std::string const& building_id, shadow::Point const& _reference_point, unsigned short const _epsg_index, std::vector<Point_3> & points, std::vector< std::vector<std::size_t> > & polygons)
            :name(building_id), reference_point(_reference_point), epsg_index(_epsg_index)
        {
            CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
            CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, surface);
            CGAL::Polygon_mesh_processing::stitch_borders(surface);
            if (!CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);
            if(!surface.empty())
                bounding_box = CGAL::Polygon_mesh_processing::bbox(surface);
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

        Bbox_3 UNode::bbox(void) const
        {
            return bounding_box;
        }

        std::string UNode::get_name(void) const
        {
            return name;
        }

        unsigned short UNode::get_epsg(void) const noexcept
        {
            return epsg_index;
        }

        shadow::Point UNode::get_reference_point(void) const noexcept
        {
            return reference_point;
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

        UNode::Halfedge_iterator UNode::prunable(void)
        {
            return std::find_if(
                halfedges_begin(),
                halfedges_end(),
                [this](Polyhedron::Halfedge & halfedge)
                {
                    bool joinable = !halfedge.is_border_edge();
                    if(joinable)
                    {
                        joinable = (
                            CGAL::cross_product(
                                CGAL::Polygon_mesh_processing::compute_face_normal(halfedge.facet(), surface),
                                CGAL::Polygon_mesh_processing::compute_face_normal(halfedge.opposite()->facet(), surface)
                            )
                            ==
                            CGAL::NULL_VECTOR
                        );
                    }
                    return  joinable;
                }
            );
        }
        std::vector<UNode::Halfedge_handle> UNode::combinable(Facet & facet) const
        {
            std::vector<UNode::Halfedge_handle> combining_edges;
            combining_edges.reserve(facet.facet_degree());

            Polyhedron::Halfedge_around_facet_circulator facet_circulator = facet.facet_begin();
            do
            {
                if(!facet_circulator->is_border_edge())
                {
                    Point_3 A(facet_circulator->vertex()->point()),
                            B(facet_circulator->next()->vertex()->point()),
                            C(facet_circulator->next()->next()->vertex()->point()),
                            D(facet_circulator->opposite()->next()->vertex()->point());
                    if(CGAL::determinant(B - A, C - A, D - A) == Kernel::FT(0))
                        combining_edges.push_back(facet_circulator->opposite());
                }
            }while(++facet_circulator != facet.facet_begin());

            return combining_edges;
        }
        std::vector<UNode::Halfedge_handle> UNode::pruning_halfedges(void)
        {
            std::vector<UNode::Halfedge_handle> combining_edges;

            std::for_each(
                facets_begin(),
                facets_end(),
                [&combining_edges, this](Facet & facet)
                {
                    auto buffer = combinable(facet);
                    std::copy_if(
                        std::begin(buffer),
                        std::end(buffer),
                        std::back_inserter(combining_edges),
                        [&combining_edges](UNode::Halfedge_handle const& h)
                        {
                            return std::none_of(
                                std::begin(combining_edges),
                                std::end(combining_edges),
                                [&h](UNode::Halfedge_handle const& present)
                                {
                                    return  (present->vertex()->point() == h->vertex()->point() && present->opposite()->vertex()->point() == h->opposite()->vertex()->point())
                                            ||
                                            (present->opposite()->vertex()->point() == h->vertex()->point() && present->vertex()->point() == h->opposite()->vertex()->point());
                                }
                            );
                        }
                    );
                }
            );
            return combining_edges;
        }
        UNode & UNode::join_facet(UNode::Halfedge_handle & h)
        {
            surface.join_facet(h);
            stitch_borders();
            return *this;
        }
        void UNode::stitch_borders(void)
        {
            if (!CGAL::Polygon_mesh_processing::is_outward_oriented(surface))
                CGAL::Polygon_mesh_processing::reverse_face_orientations(surface);

            CGAL::Polygon_mesh_processing::stitch_borders(surface);
        }

        Point_3 UNode::centroid(UNode::Facet_const_handle facet) const
        {
            Polyhedron::Halfedge_around_facet_const_circulator circulator = facet->facet_begin();
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
            UNode::Facet _face = *facet;
            ExactToInexact to_inexact;
            auto a = CGAL::Polygon_mesh_processing::face_area(&_face, surface);
            std::cout << a << std::endl;
            return to_inexact(a);
        }
        
        std::vector<UNode::Facet_const_handle> UNode::facet_adjacents(UNode::Facet const& facet) const
        {
            std::vector<UNode::Facet_const_handle> adjacents;
            adjacents.reserve(facet.facet_degree());

            auto circulator = facet.facet_begin();
            do
            {
                auto buff = circulator->opposite()->facet();
                if(!circulator->is_border() && buff != NULL)
                    adjacents.push_back(buff);
            }while(++circulator != facet.facet_begin());
            return adjacents;
        }
        std::vector<bool> UNode::facet_adjacency_matrix(void) const
        {
            std::vector<bool> adjacency(facets_size() * facets_size(), false);
            return facet_adjacency_matrix(adjacency, 0);
        }
        std::vector<bool> & UNode::facet_adjacency_matrix(std::vector<bool> & matrix, std::size_t offset) const
        {
            std::size_t n = static_cast<std::size_t>(std::floor(std::sqrt(matrix.size())));
            std::size_t size = facets_size();

            if(n * n != matrix.size())
                throw std::logic_error("The adjacency matrix must be square!");
            if(n < offset + facets_size())
                throw std::underflow_error("The matrix cannot hold the whole brick!");

            std::vector<UNode::Facet_const_handle> facets(size);
            std::transform(
                facets_cbegin(),
                facets_cend(),
                std::begin(facets),
                [](UNode::Facet const& facet)
                {
                    return &facet;
                }
            );

            for(std::size_t diag(offset); diag != size + offset; ++diag)
                matrix.at(diag * size + diag) = true;

            std::vector<UNode::Facet_const_handle> line_adjacents;
            for(std::size_t line(0); line != size; ++line)
            {
                line_adjacents = facet_adjacents(*facets.at(line));

                for(auto adjacent : line_adjacents)
                {
                    auto placeholder = std::find(std::begin(facets), std::end(facets), adjacent);
                    if(placeholder != std::end(facets))
                    {
                        std::size_t index = static_cast<std::size_t>(std::distance(std::begin(facets), placeholder)) + offset;
                        matrix.at((line + offset) * size + index) = true;
                    }
                }

                line_adjacents.clear();
            }   
            return matrix;
        }

        std::ostream & operator <<(std::ostream &os, UNode const& unode)
        {
            os  << "# Name: " << unode.name << std::endl
                << unode.surface;
            return os;
        }

        io::Adjacency_stream & operator <<(io::Adjacency_stream & as, UNode const& unode)
        {
            std::cout << unode <<std::endl;
            std::for_each(
                unode.facets_cbegin(),
                unode.facets_cend(),
                [&as, &unode](UNode::Facet const& facet)
                {
                    std::cout << facet.facet_degree() << " " << unode.normal(facet.halfedge()->facet()) << std::endl;
                    as << facet.facet_degree() << " " << unode.area(facet.halfedge()->facet()) << " " << unode.centroid(facet.halfedge()->facet()) << unode.normal(facet.halfedge()->facet()) << std::endl;
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
    }
}
