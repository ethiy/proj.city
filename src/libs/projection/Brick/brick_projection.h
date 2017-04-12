#pragma once

#include "../../geometry_definitions.h"
#include "../../shadow/Point/point.h"
#include "../Face/face_projection.h"

#include <ogrsf_frmts.h>

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
            BrickPrint(const std::string & _name, const Bbox_3 & _bounding_box, const shadow::Point & reference_point, unsigned short espg_index);
            BrickPrint(const std::string & _name, OGRLayer* projection_layer);
            BrickPrint(const BrickPrint & other);
            BrickPrint(BrickPrint && other);
            ~BrickPrint(void);

            void swap(BrickPrint & other);
            
            BrickPrint & operator=(const BrickPrint & other);
            BrickPrint & operator=(BrickPrint && other);

            BrickPrint & operator+=(const BrickPrint & other);

            Bbox_2 bbox(void) const noexcept;
            std::string get_name(void) const noexcept;
            size_t size(void) const noexcept;

            typedef std::list<FacePrint>::iterator iterator;
            typedef std::list<FacePrint>::const_iterator const_iterator;
            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            bool in_domain(const Point_2 & point) const;
            bool contains(const Point_2 & point) const;
            bool contains(const InexactPoint_2 & inexact_point) const;
            bool contains(const FacePrint & facet) const;
            bool overlaps(const Polygon & polygon) const;
            bool overlaps(const Polygon_with_holes & polygon) const;
            bool overlaps(const FacePrint & facet) const;
            bool is_under(const FacePrint &) const;
            bool check_integrity(void) const;

            bool has_same_footprint(const BrickPrint & other) const;
            bool has_same_facets(const BrickPrint & other) const;

            void insert(const FacePrint & facet);

            double get_height(const Point_2 &) const;
            double get_height(const InexactPoint_2 & inexact_point) const;
            double get_mean_height(const Polygon & window) const;

            void to_ogr(GDALDataset* file) const;
        private:
            std::string name;
            Bbox_2 bounding_box;
            shadow::Point reference_point;
            unsigned short espg_index = 2154;
            std::list<FacePrint> projected_facets;
            Polygon_set projected_surface;

            friend std::ostream & operator<<(std::ostream & os, const BrickPrint & brick_projection);
        };

        BrickPrint & operator+(BrickPrint & lhs, const BrickPrint & rhs);
        bool operator==(const BrickPrint & lhs, const BrickPrint & rhs);
        bool operator!=(const BrickPrint & lhs, const BrickPrint & rhs);
    }

    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs);
}
