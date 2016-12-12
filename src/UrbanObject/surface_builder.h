#pragma once

#include "urban_object.h"

#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <algorithm>
#include <iterator>


namespace urban
{
    template <class HDS>
    class SurfaceBuilder: public CGAL::Modifier_base<HDS>
    {
    public:
        SurfaceBuilder(ShadowMesh _shadow_mesh): shadow_mesh(_shadow_mesh){}
        void operator()(HDS & target)
        {
            std::map<size_t, Point> points = shadow_mesh.get_points();
            std::map<size_t, Face> faces = shadow_mesh.get_faces();

            CGAL::Polyhedron_incremental_builder_3<HDS> incremental_builder( target, true);
            incremental_builder.begin_surface(points.size(), faces.size());

            std::for_each(
                std::begin(points),
                std::end(points),
                [&](std::pair<size_t, Point> p)
                {
                    incremental_builder.add_vertex( p.second);
                }
            );
            std::for_each(
                std::begin(faces),
                std::end(faces),
                [&](std::pair<size_t, Face> face)
                {
                    incremental_builder.begin_facet();
                    std::for_each(
                        std::begin(face.second),
                        std::end(face.second),
                        [&](size_t index)
                        {
                            incremental_builder.add_vertex_to_facet(index);
                        } 
                    );
                    incremental_builder.end_facet();
                }
            );
            incremental_builder.end_surface();
        }
    private:
        ShadowMesh shadow_mesh;
    };
}
