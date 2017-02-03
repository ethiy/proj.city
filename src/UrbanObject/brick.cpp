#include "brick.h"
#include "surface_builder.h"

#include <CGAL/IO/Polyhedron_iostream.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#endif // CGAL_USE_GEOMVIEW

#include <vector>

namespace urban
{
    Brick::Brick(void) {}

    Brick::Brick(ShadowMesh mesh): name(mesh.get_name()), bounding_box(mesh.bbox())
    {
        SurfaceBuilder<Polyhedron::HalfedgeDS> builder(mesh);
        surface.delegate(builder);
    }

    Brick::Brick(const Brick &other) : name(other.name), surface(other.surface), bounding_box(other.bounding_box) {}

    Brick::~Brick(void) {}

    std::string Brick::get_name(void) const noexcept
    {
        return name;
    }

    Bbox Brick::bbox(void) const noexcept
    {
        return bounding_box;
    }

    size_t Brick::vertices_number(void) const
    {
        return surface.size_of_vertices();
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

    void Brick::set_color(Color color)
    {
        std::for_each(
            surface.facets_begin(),
            surface.facets_end(),
            [&](Facet &facet) {
                facet.set_color(color);
            });
    }

    std::ostream &operator<<(std::ostream &os, const Brick &uobj)
    {
        os << "# Name: " << uobj.name << std::endl
        << uobj.surface;
        return os;
    }

    #ifdef CGAL_USE_GEOMVIEW
    CGAL::Geomview_stream &operator<<(CGAL::Geomview_stream &gs, const Brick &uobj)
    {
        gs << uobj.surface;
        return gs;
    }
    #endif // CGAL_USE_GEOMVIEW
}
