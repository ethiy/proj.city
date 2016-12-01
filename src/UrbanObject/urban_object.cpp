#include "urban_object.h"
#include "surface_builder.h"

#include <vector>

#include <CGAL/IO/Polyhedron_iostream.h>

namespace urban
{
    UrbanObject::UrbanObject(void){}

    UrbanObject::UrbanObject(_Mesh mesh)
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
}
