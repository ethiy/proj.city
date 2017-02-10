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

        
        bool contains(const Polygon_with_holes &) const;
        bool is_under(const FaceProjection &) const;
        void push_facet(FaceProjection &);

        bool in_domain(const Point_2 &) const;
        double get_height(const Point_2 &) const;
    private:
        std::string name;
        std::vector<FaceProjection> facets_xy;
        Polygon_with_holes projected_surface;
        Bbox_2 bounding_box;
    };
}
