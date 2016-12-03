#include "urban_object.h"
#include "surface_builder.h"

#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_geomview_ostream.h>

#include <vector>

namespace urban
{
    UrbanObject::UrbanObject(void){}

    UrbanObject::UrbanObject(Mesh mesh)
    {
        SurfaceBuilder<Polyhedron::HalfedgeDS> builder(mesh);
        surface.delegate( builder);
    }

    UrbanObject::UrbanObject(const UrbanObject & other): surface(other.surface){}

    UrbanObject::~UrbanObject(void){}

    std::ostream& operator<<(std::ostream & os, const UrbanObject & uobj)
    {
        CGAL::set_pretty_mode( os);
        os << uobj.surface;
        return os;
    }

    CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream & gs, const UrbanObject & uobj)
    {
        gs << uobj.surface;
        return gs;
    }
}
