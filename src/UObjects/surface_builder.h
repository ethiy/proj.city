#pragma once

#include "urban_objects.h"

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Vector_3.h>

#include <algorithm>


template <class HDS>
class SurfaceBuilder: public CGAL::Modifier_base<HDS>
{
public:
    SurfaceBuilder(std::vector<Lib3dsFace> mesh_faces, std::map<size_t,Lib3dsVector> mesh_points): faces(mesh_faces), points(mesh_points){}
    void operator()(HDS & target)
    {
        CGAL::Polyhedron_incremental_builder_3<HDS> incremental_builder( target, true);
        incremental_builder.begin_surface(points.size(), faces.size());
        std::for_each( faces.begin(), faces.end(), [&](Lib3dsFace face)
                                                            {
                                                                Point point_0 = Point(points[face.points[0]][0], points[face.points[0]][1], points[face.points[0]][2]);
                                                                Point point_1 = Point(points[face.points[1]][0], points[face.points[1]][1], points[face.points[1]][2]);
                                                                Point point_2 = Point(points[face.points[2]][0], points[face.points[2]][1], points[face.points[2]][2]);

                                                                incremental_builder.add_vertex( point_0);
                                                                incremental_builder.add_vertex( point_1);
                                                                incremental_builder.add_vertex( point_2);

                                                                Vector v1 = Vector(point_0, point_1);
                                                                Vector v2 = Vector(point_1, point_2);
                                                                Vector n = Vector(face.normal[0], face.normal[1], face.normal[2]);
                                                                if(CGAL::determinant(v1, v2, n)>0)
                                                                {
                                                                    incremental_builder.begin_facet();
                                                                    incremental_builder.add_vertex_to_facet( 0);
                                                                    incremental_builder.add_vertex_to_facet( 1);
                                                                    incremental_builder.add_vertex_to_facet( 2);
                                                                    incremental_builder.end_facet();
                                                                }
                                                            }
                                                            );
        incremental_builder.end_surface();
    }
private:
    std::map<size_t,Lib3dsVector> points;
    std::vector<Lib3dsFace> faces;
};
