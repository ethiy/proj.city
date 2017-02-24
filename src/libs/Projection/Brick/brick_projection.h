#pragma once

#include "../../geometry_definitions.h"
#include "../Face/face_projection.h"

#include <string>
#include <list>

#include <ostream>

namespace urban
{
    namespace projection
    {
        class BrickPrint
        {
        public:
            BrickPrint(void);
            BrickPrint(const std::string & _name);
            BrickPrint(const BrickPrint &other);
            BrickPrint(BrickPrint && other);
            ~BrickPrint(void);

            void swap(BrickPrint & other);
            
            BrickPrint operator=(const BrickPrint & other);

            BrickPrint operator=(BrickPrint && other);

            Bbox_2 bbox(void) const;

            typedef std::list<FacePrint>::iterator iterator;
            typedef std::list<FacePrint>::const_iterator const_iterator;
            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            
            bool contains(const Polygon_with_holes & facet) const;
            bool overlaps(const Polygon_with_holes & facet) const;
            bool is_under(const FacePrint &) const;
            void push_facet(FacePrint &);

            bool in_domain(const Point_2 &) const;
            double get_height(const Point_2 &) const;

            void plot(void) const;
        private:
            std::string name;
            std::list<FacePrint> projected_facets;
            Polygon_set projected_surface;

            friend std::ostream & operator<<(std::ostream & os, const BrickPrint & brick_projection);
        };
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs);
}
