#include "urban_object.h"
#include "surface_builder.h"

#include <vector>

#include <CGAL/IO/Polyhedron_iostream.h>


UrbanObject::UrbanObject(){}

UrbanObject::UrbanObject(Lib3dsMesh lib3ds_mesh)
{
    // Copy 3ds facets and points to cxx like structures
    std::map<size_t, Point> mesh_points;
    for(size_t it=0; it<lib3ds_mesh.points; ++it)
    {
        Lib3dsPoint current = *(lib3ds_mesh.pointL+it);
        mesh_points[it] = Point(current.pos[0], current.pos[1], current.pos[2]);
    }
    std::vector<Lib3dsFace> faces( lib3ds_mesh.faceL, lib3ds_mesh.faceL + lib3ds_mesh.faces);

    // Build mesh in Polyhedron_3
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
