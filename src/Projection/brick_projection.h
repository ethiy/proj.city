#pragma once

#include "../geometry_definitions.h"
#include "face_projection.h"

#include <string>
#include <map>

namespace urban
{
    class BrickProjection
    {
    public:
        BrickProjection(void);
        BrickProjection(const std::string &, const std::map<size_t, FaceProjection> &, const Bbox_2 &);
        BrickProjection(const std::string &, const Bbox &);
        BrickProjection(BrickProjection &);
        ~BrickProjection(void);

        void set_name(const std::string &);
        void set_Bbox(const Bbox &);

        Bbox_2 bbox(void);

        void add_facet_projection(const FaceProjection &);

        bool in_domain(const Point_2 &);
        double get_height(const Point_2 &);
    private:
        std::string name;
        std::map<size_t, FaceProjection> facets_xy;
        Bbox_2 bounding_box;
    };
}