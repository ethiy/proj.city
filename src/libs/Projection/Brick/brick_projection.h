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
            BrickPrint(const std::string & _name, const Bbox_3 & _bounding_box);
            BrickPrint(const FacePrint & face_projection);
            BrickPrint(const BrickPrint &other);
            BrickPrint(BrickPrint && other);
            ~BrickPrint(void);

            void swap(BrickPrint & other);
            
            BrickPrint & operator=(const BrickPrint & other);
            BrickPrint & operator=(BrickPrint && other);

            BrickPrint & operator+=(const BrickPrint & other);

            Bbox_2 bbox(void);

            size_t size(void) const noexcept;
            typedef std::list<FacePrint>::iterator iterator;
            typedef std::list<FacePrint>::const_iterator const_iterator;
            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            
            bool contains(const FacePrint & facet) const;
            bool overlaps(const FacePrint & facet) const;
            bool is_under(const FacePrint &) const;
            bool check_integrity(void) const;

            void insert(const FacePrint & facet);

            bool in_domain(const Point_2 &) const;
            double get_height(const Point_2 &) const;

            void to_ogr(GDALDataset* file) const;
        private:
            std::string name;
            std::list<FacePrint> projected_facets;
            Polygon_set projected_surface;
            Bbox_2 bounding_box;

            friend std::ostream & operator<<(std::ostream & os, const BrickPrint & brick_projection);
        };

        BrickPrint & operator+(BrickPrint & lhs, const BrickPrint & rhs);
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs);
}
