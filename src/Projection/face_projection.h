#pragma once

#include "../geometry_definitions.h"

namespace urban
{
    class FaceProjection : urban::Polygon
    {
    public:
        Plane get_plane(void);
        Vector get_normal(void);
        double get_height(Point_2 &);

        bool is_inside(Point_2 &);

        void set_plane(Plane & _plane);
    private:
        Plane original_plane;
    };
}