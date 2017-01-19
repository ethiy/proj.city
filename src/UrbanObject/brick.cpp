#include "brick.h"
#include "surface_builder.h"

#include <CGAL/IO/Polyhedron_iostream.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#endif // CGAL_USE_GEOMVIEW

#include <vector>

namespace urban
{
    Brick::Brick(void){}

    Brick::Brick(ShadowMesh mesh)
    {
        name = mesh.get_name();
        SurfaceBuilder<Polyhedron::HalfedgeDS> builder(mesh);
        surface.delegate( builder);
    }

    Brick::Brick(const Brick & other): surface(other.surface){}

    Brick::~Brick(void){}

    std::string Brick::get_name(void) const noexcept
    {
        return name;
    }

    Point Brick::barycenter(void)
    {
        Vector barycenter( 0.0, 0.0, 0.0);
        std::cerr << "Nor yet implemented!" << std::endl;
        return CGAL::ORIGIN + barycenter;
    }

    void Brick::set_color(Color color)
    {
        std::for_each(
            surface.facets_begin(),
            surface.facets_end(),
            [&](Facet & facet)
            {
                facet.set_color(color);
            }
        );
    }

    std::ostream& operator<<(std::ostream & os, const Brick & uobj)
    {
        os << "# Name: " << uobj.name << std::endl
           << uobj.surface;
        return os;
    }

    #ifdef CGAL_USE_GEOMVIEW
    CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream & gs, const Brick & uobj)
    {
        gs << uobj.surface;
        return gs;
    }
    #endif // CGAL_USE_GEOMVIEW
}
