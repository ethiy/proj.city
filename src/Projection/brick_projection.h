#pragma once

#include "../geometry_definitions.h"
#include "face_projection.h"

#include <string>

namespace urban
{
    class BrickProjection
    {
    public:
        BrickProjection(void);
        BrickProjection(std::string, std::map<size_t, FaceProjection>, Bbox_2);
        BrickProjection(BrickProjection &);
        ~BrickProjection(void);

        bool in_domain(Point_2 &);
        double get_height(Point_2 &);
    private:
        std::string name;
        std::map<size_t, FaceProjection> facets_xy;
        Bbox_2 bounding_box;
    };
}