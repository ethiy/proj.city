#include "urban_objects.h"

#include <CGAL/Polyhedron_incremental_builder_3.h>

template <class HDS>
class SurfaceBuilder: public CGAL::Modifier_base<HDS>
{
public:
    SurfaceBuilder(std::vector<Lib3dsFace> mesh_faces, std::map<size_t,Lib3dsVector> mesh_points): faces(mesh_faces), points(mesh_points){}
    void operator()(HDS & target)
    {
        CGAL::Polyhedron_incremental_builder_3<HDS> incremental_builder( target, true);
        incremental_builder.begin_surface(points.size(), facets.size());
        incremental_builder.end_surface();
    }
private:
    std::map<size_t,Lib3dsVector> points;
    std::vector<Lib3dsFace> faces;
};
