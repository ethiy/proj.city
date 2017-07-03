#pragma once

#include "brick.h"

#include <CGAL/Polyhedron_incremental_builder_3.h>

#include <algorithm>
#include <iterator>

namespace urban
{
    template <class HDS>
    class SurfaceBuilder: public CGAL::Modifier_base<HDS>
    {
    public:
        SurfaceBuilder(shadow::Mesh const& _shadow_mesh, bool _centered, shadow::Point _pivot)
            : centered(_centered), pivot(_pivot), shadow_mesh(_shadow_mesh){}

        void operator()(HDS & target)
        {
            CGAL::Polyhedron_incremental_builder_3<HDS> incremental_builder(target, true);
            incremental_builder.begin_surface(shadow_mesh.points_size(), shadow_mesh.faces_size());
            InexactToExact to_exact;

            std::for_each(
                shadow_mesh.points_cbegin(),
                shadow_mesh.points_cend(),
                [&incremental_builder, &to_exact, this](std::pair<std::size_t, shadow::Point> const& p)
                {
                    incremental_builder.add_vertex(
                        Point_3(
                            to_exact(
                                CGAL::Point_3<InexactKernel>(
                                    p.second.x() - !centered * pivot.x(),
                                    p.second.y() - !centered * pivot.y(),
                                    p.second.z()
                                )
                            )
                        )
                    );
                }
            );
            std::for_each(
                shadow_mesh.faces_cbegin(),
                shadow_mesh.faces_cend(),
                [&incremental_builder, this](std::pair<std::size_t, shadow::Face> const& face)
                {
                    incremental_builder.begin_facet();
                    std::for_each(
                        std::begin(face.second),
                        std::end(face.second),
                        [&incremental_builder](std::size_t index)
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
        bool centered;
        shadow::Point pivot;
        shadow::Mesh shadow_mesh;
    };
}
