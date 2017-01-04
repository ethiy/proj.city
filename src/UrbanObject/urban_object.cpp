#include "urban_object.h"
#include "surface_builder.h"

#include <CGAL/IO/Polyhedron_iostream.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#endif // CGAL_USE_GEOMVIEW

#include <vector>

namespace urban
{
    UrbanObject::UrbanObject(void){}

    UrbanObject::UrbanObject(ShadowMesh mesh)
    {
        name = mesh.get_name();
        SurfaceBuilder<Polyhedron::HalfedgeDS> builder(mesh);
        surface.delegate( builder);
    }

    UrbanObject::UrbanObject(const UrbanObject & other): surface(other.surface){}

    UrbanObject::~UrbanObject(void){}

    std::string UrbanObject::get_name(void) const noexcept
    {
        return name;
    }

    std::ostream& operator<<(std::ostream & os, const UrbanObject & uobj)
    {
        os << "# Name: " << uobj.name << std::endl
           << uobj.surface;
        return os;
    }

    #ifdef CGAL_USE_GEOMVIEW
    CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream & gs, const UrbanObject & uobj)
    {
        gs << uobj.surface;
        return gs;
    }
    #endif // CGAL_USE_GEOMVIEW
}
