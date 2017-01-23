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
    private:
        std::string name;
        std::map<size_t, FaceProjection> bricks_xy;
        Bbox_2 bounding_box;
    };
}