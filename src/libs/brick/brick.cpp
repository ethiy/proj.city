#include "brick.h"
#include "surface_builder.h"

#include <CGAL/IO/Polyhedron_iostream.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#endif // CGAL_USE_GEOMVIEW

#include <vector>

namespace urban
{
    Brick::Brick(void): name("no name"){}
    Brick::Brick(Brick const& other)
        :name(other.name), reference_point(other.reference_point), epsg_index(other.epsg_index), surface(other.surface), bounding_box(other.bounding_box){}
    Brick::Brick(Brick && other)
        : name(std::move(other.name)), reference_point(std::move(other.reference_point)), epsg_index(std::move(other.epsg_index)), surface(std::move(other.surface)), bounding_box(std::move(other.bounding_box)){}
    Brick::Brick(shadow::Mesh const& mesh, shadow::Point const& _reference_point)
        : name(mesh.get_name()), reference_point(_reference_point), bounding_box(mesh.bbox().to_cgal())
    {
        SurfaceBuilder<Polyhedron::HalfedgeDS> builder(mesh);
        surface.delegate(builder);
    }
    Brick::Brick(shadow::Mesh const& mesh, shadow::Point const& _reference_point, unsigned short _epsg_index)
        : name(mesh.get_name()), reference_point(_reference_point), epsg_index(_epsg_index), bounding_box(mesh.bbox().to_cgal())
    {
        SurfaceBuilder<Polyhedron::HalfedgeDS> builder(mesh);
        surface.delegate(builder);
    }
    Brick::~Brick(void){}

    void Brick::swap(Brick & other)
    {
        using std::swap;
        swap(name, other.name);
        swap(surface, other.surface);
        swap(bounding_box, other.bounding_box);
    }

    Brick & Brick::operator=(Brick const& other) noexcept
    {
        name = other.name;
        reference_point = other.reference_point;
        epsg_index = other.epsg_index;
        surface = other.surface;
        bounding_box = other.bounding_box;
        return *this;
    }

    Brick & Brick::operator=(Brick && other) noexcept
    {
        name = std::move(other.name);
        reference_point = std::move(other.reference_point);
        epsg_index = std::move(other.epsg_index);
        surface = std::move(other.surface);
        bounding_box = std::move(other.bounding_box);
        return *this;
    }

    std::string Brick::get_name(void) const noexcept
    {
        return name;
    }

    Bbox_3 Brick::bbox(void) const noexcept
    {
        return bounding_box;
    }

    unsigned short Brick::get_epsg(void) const noexcept
    {
        return epsg_index;
    }

    shadow::Point Brick::get_reference_point(void) const noexcept
    {
        return reference_point;
    }

    std::size_t Brick::vertices_size(void) const
    {
        return surface.size_of_vertices();
    }

    std::size_t Brick::facets_size(void) const
    {
        return surface.size_of_facets();
    }

    Brick::Facet_iterator Brick::facets_begin(void) noexcept
    {
        return surface.facets_begin();
    }
    Brick::Facet_iterator Brick::facets_end(void) noexcept
    {
        return surface.facets_end();
    }
    Brick::Facet_const_iterator Brick::facets_cbegin(void) const noexcept
    {
        return surface.facets_begin();
    }
    Brick::Facet_const_iterator Brick::facets_cend(void) const noexcept
    {
        return surface.facets_end();
    }

    Brick::Halfedge_iterator Brick::halfedges_begin(void) noexcept
    {
        return surface.halfedges_begin();
    }
    Brick::Halfedge_iterator Brick::halfedges_end(void) noexcept
    {
        return surface.halfedges_end();
    }
    Brick::Halfedge_const_iterator Brick::halfedges_cbegin(void) const noexcept
    {
        return surface.halfedges_begin();
    }
    Brick::Halfedge_const_iterator Brick::halfedges_cend(void) const noexcept
    {
        return surface.halfedges_end();
    }

    Brick::Halfedge_iterator Brick::border_halfedges_begin(void) noexcept
    {
        return surface.border_halfedges_begin();
    }

    Brick::Halfedge_const_iterator Brick::border_halfedges_begin(void) const noexcept
    {
        return surface.border_halfedges_begin();
    }


    Brick::Point_iterator Brick::points_begin(void) noexcept
    {
        return surface.points_begin();
    }
    Brick::Point_iterator Brick::points_end(void) noexcept
    {
        return surface.points_end();
    }
    Brick::Point_const_iterator Brick::points_cbegin(void) const noexcept
    {
        return surface.points_begin();
    }
    Brick::Point_const_iterator Brick::points_cend(void) const noexcept
    {
        return surface.points_end();
    }

    Brick::Plane_iterator Brick::planes_begin(void) noexcept
    {
        return surface.planes_begin();
    }
    Brick::Plane_iterator Brick::planes_end(void) noexcept
    {
        return surface.planes_end();
    }
    Brick::Plane_const_iterator Brick::planes_cbegin(void) const noexcept
    {
        return surface.planes_begin();
    }
    Brick::Plane_const_iterator Brick::planes_cend(void) const noexcept
    {
        return surface.planes_end();
    }

    Brick::Halfedge_iterator Brick::prunable(void)
    {
        return std::find_if(
            halfedges_begin(),
            halfedges_end(),
            [](Polyhedron::Halfedge const& halfedge)
            {
                bool joignable = !halfedge.is_border_edge();
                if(joignable)
                {
                    Point_3 A(halfedge.vertex()->point()),
                            B(halfedge.next()->vertex()->point()),
                            C(halfedge.next()->next()->vertex()->point()),
                            D(halfedge.opposite()->next()->vertex()->point());
                    joignable = (std::abs(to_double(CGAL::determinant(B - A, C - A, D - A))) < std::numeric_limits<double>::epsilon());
                }
                return  joignable;
            }
        );
    }

    std::vector<Brick::Halfedge_handle> Brick::combinable(Facet & facet) const
    {
        std::vector<Brick::Halfedge_handle> combining_edges;
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
                if(std::abs(to_double(CGAL::determinant(B - A, C - A, D - A))) < std::numeric_limits<double>::epsilon())
                    combining_edges.push_back(facet_circulator->opposite());
            }
        }while(++facet_circulator != facet.facet_begin());

        return combining_edges;
    }

    std::vector<Brick::Halfedge_handle> Brick::pruning_halfedges(void)
    {
        std::vector<Brick::Halfedge_handle> combining_edges;
        std::vector<Brick::Halfedge_handle> buffer;

        std::for_each(
            facets_begin(),
            facets_end(),
            [&combining_edges, &buffer, this](Facet & facet)
            {
                buffer = combinable(facet);
                std::copy_if(
                    std::begin(buffer),
                    std::end(buffer),
                    std::back_inserter(combining_edges),
                    [&combining_edges](Brick::Halfedge_handle const& h)
                    {
                        return std::none_of(
                            std::begin(combining_edges),
                            std::end(combining_edges),
                            [&h](Brick::Halfedge_handle const& present)
                            {
                                return  (present->vertex()->point() == h->vertex()->point() && present->opposite()->vertex()->point() == h->opposite()->vertex()->point())
                                        ||
                                        (present->opposite()->vertex()->point() == h->vertex()->point() && present->vertex()->point() == h->opposite()->vertex()->point());
                            }
                        );
                    }
                );
                buffer.clear();
            }
        );
        return combining_edges;
    }

    Brick & Brick::join_facet(Brick::Halfedge_handle & h)
    {
        surface.join_facet(h);
        return *this;
    }

    Point_3 Brick::centroid(Brick::Facet const& facet) const
    {
        Polyhedron::Halfedge_around_facet_const_circulator circulator = facet.facet_begin();
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
        }while(circulator != facet.facet_begin());

        return CGAL::ORIGIN + centroid / area(facet);
    }

    Vector_3 Brick::normal(Brick::Facet const& facet) const
    {
        Polyhedron::Halfedge_around_facet_const_circulator circulator = facet.facet_begin();
        return CGAL::normal(circulator->vertex()->point(), circulator->next()->vertex()->point(), circulator->next()->next()->vertex()->point());
    }

    double Brick::area(Brick::Facet const& facet) const
    {
        Polyhedron::Halfedge_around_facet_const_circulator circulator = facet.facet_begin();
        Vector_3 n = normal(facet);

        double area(0);
        do
        {
            area += to_double(CGAL::cross_product(circulator->vertex()->point() - CGAL::ORIGIN, circulator->next()->vertex()->point() - CGAL::ORIGIN) * n/2.);
        }while(circulator != facet.facet_begin());

        return area;
    }
    
    std::vector<Brick::Facet_const_handle> Brick::facet_adjacents(Brick::Facet const& facet) const
    {
        std::vector<Brick::Facet_const_handle> adjacents;
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

    std::vector<bool> Brick::facet_adjacency_matrix(void) const
    {
        std::size_t&& n = facets_size();
        std::vector<Brick::Facet_const_handle> facets(n);
        std::transform(
            facets_cbegin(),
            facets_cend(),
            std::begin(facets),
            [](Brick::Facet const& facet)
            {
                return &facet;
            }
        );
        std::vector<bool> adjacency(n * n);

        for(std::size_t diag(0); diag != n; ++diag)
            adjacency.at(diag * n + diag) = true;

        std::vector<Brick::Facet_const_handle> line_adjacents;
        for(std::size_t line(0); line != n; ++line)
        {
            line_adjacents = facet_adjacents(*facets.at(line));

            for(auto adjacent : line_adjacents)
            {
                auto placeholder = std::find(std::begin(facets), std::end(facets), adjacent);
                if(placeholder != std::end(facets))
                {
                    long index = std::distance(std::begin(facets), placeholder);
                    adjacency.at(line * n + index) = true;
                }
            }

            line_adjacents.clear();
        }   

        return adjacency;
    }

    std::ostream & operator<<(std::ostream &os, Brick const& brick)
    {
        os  << "# Name: " << brick.name << std::endl
            << brick.surface;
        return os;
    }

    io::Adjacency_stream & operator<<(io::Adjacency_stream & as, Brick const& brick)
    {
        std::for_each(
            brick.facets_cbegin(),
            brick.facets_cend(),
            [&as, &brick](Brick::Facet const& facet)
            {
                as << facet.facet_degree() << " " << brick.area(facet) << " " << brick.centroid(facet) << brick.normal(facet) << std::endl;
            }
        );

        std::vector<bool> matrix = brick.facet_adjacency_matrix();

        as << matrix << std::endl;

        return as;
    }

    #ifdef CGAL_USE_GEOMVIEW
    CGAL::Geomview_stream & operator<<(CGAL::Geomview_stream &gs, Brick const& brick)
    {
        gs << brick.surface;
        return gs;
    }
    #endif // CGAL_USE_GEOMVIEW

    void swap(Brick & lhs, Brick &rhs)
    {
        lhs.swap(rhs);
    }
}
