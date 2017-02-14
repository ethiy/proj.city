#pragma once

#include "../geometry_definitions.h"
#include "face_projection.h"

#include <string>
#include <list>

namespace urban
{
    namespace projection
    {
        class BrickPrint
        {
        public:
            BrickPrint(void);
            BrickPrint(const std::string & _name, const Bbox & _bounding_box);
            BrickPrint(const BrickPrint &other);
            BrickPrint(BrickPrint && other);
            ~BrickPrint(void);

            void swap(BrickPrint & other);
            
            BrickPrint operator=(const BrickPrint & other);

            BrickPrint operator=(BrickPrint && other);

            Bbox_2 bbox(void);

            typedef std::list<FacePrint>::iterator iterator;
            typedef std::list<FacePrint>::const_iterator const_iterator;
            iterator begin(void);
            iterator end(void);
            const_iterator cbegin(void);
            const_iterator cend(void);

            
            bool contains(const Polygon_with_holes &) const;
            bool is_under(const FacePrint &) const;
            void push_facet(FacePrint &);

            bool in_domain(const Point_2 &) const;
            double get_height(const Point_2 &) const;
        private:
            std::string name;
            std::list<FacePrint> projected_facets;
            Polygon_with_holes projected_surface;
            Bbox_2 bounding_box;
        };
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs);
}
