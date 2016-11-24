#include "urban_objects.h"
#include "surface_builder.h"

#include <map>
#include <vector>

#include <CGAL/IO/Polyhedron_iostream.h>


UrbanObject::UrbanObject(){}

UrbanObject::UrbanObject(Lib3dsMesh lib3ds_mesh)
{
    // Copy 3ds facets and points to cxx like structures
    std::map<size_t, Lib3dsVector> mesh_points;
    for(size_t it; it<lib3ds_mesh.points; ++it)
    {
        Lib3dsPoint current = *(lib3ds_mesh.pointL+it);
        mesh_points[it] = current.pos;
    }
    std::vector<Lib3dsFace> faces( lib3ds_mesh.faceL, lib3ds_mesh.faceL + lib3ds_mesh.faces);
    // use Polyhedron_incremental_builder_3 and Modifier_base
    SurfaceBuilder<Polyhedron::HalfedgeDS> builder(faces, mesh_points);
    surface.delegate( builder);
}

UrbanObject::UrbanObject(const UrbanObject & other): surface(other.surface){}

UrbanObject::~UrbanObject(){}

std::ostream& operator<<(std::ostream & os, const UrbanObject & uobj)
{
    CGAL::set_pretty_mode( os);
    os << uobj.surface;
    return os;
}