#pragma once

#include "../geometry_definitions.h"
#include "face_projection.h"

#include <string>
#include <vector>

namespace urban
{
    class BrickProjection
    {
    public:
        BrickProjection(void);
        BrickProjection(const std::string &, const std::vector<FaceProjection> &, const Bbox_2 &);
        BrickProjection(const std::string &, const Bbox &);
        BrickProjection(const BrickProjection &);
        ~BrickProjection(void);

        void set_name(const std::string &);
        void set_Bbox(const Bbox &);

        Bbox_2 bbox(void);

        typedef std::vector<FaceProjection>::iterator iterator;
        typedef std::vector<FaceProjection>::const_iterator const_iterator;
        iterator begin(void);
        iterator end(void);
        const_iterator cbegin(void);
        const_iterator cend(void);
        void push_facet(FaceProjection &);

        bool in_domain(const Point_2 &);
        double get_height(const Point_2 &);
    private:
        std::string name;
        std::vector<FaceProjection> facets_xy;
        Bbox_2 bounding_box;
    };
}
