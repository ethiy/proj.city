#pragma once

#include "../../geometry_definitions.h"
#include "../../shadow/Point/point.h"
#include "../face/face_projection.h"

#include <ogrsf_frmts.h>

#include <string>
#include <vector>

#include <ostream>

namespace urban
{
    namespace projection
    {
        class BrickPrint
        {
        public:
            BrickPrint(void);
            BrickPrint(FacePrint const& face_projection);
            BrickPrint(BrickPrint const& other);
            BrickPrint(BrickPrint && other);
            ~BrickPrint(void);

            void swap(BrickPrint & other);
            
            BrickPrint & operator =(BrickPrint const& other);
            BrickPrint & operator =(BrickPrint && other);

            BrickPrint & operator +=(BrickPrint const& other);

            Bbox_2 bbox(void) const noexcept;
            std::size_t size(void) const noexcept;

            typedef std::vector<FacePrint>::iterator iterator;
            typedef std::vector<FacePrint>::const_iterator const_iterator;
            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            bool in_domain(Point_2 const& point) const;
            bool contains(Point_2 const& point) const;
            bool contains(InexactPoint_2 const& inexact_point) const;
            bool contains(FacePrint const& facet) const;
            bool overlaps(Polygon const& polygon) const;
            bool overlaps(Polygon_with_holes const& polygon) const;
            bool overlaps(FacePrint const& facet) const;
            bool is_under(FacePrint const&) const;
            bool check_integrity(void) const;

            double get_height(Point_2 const&) const;
            double get_height(InexactPoint_2 const& inexact_point) const;

            std::vector<FacePrint> occlusion(FacePrint const& new_facet);
            BrickPrint & occlusion(BrickPrint & other);
        private:
            Bbox_2 bounding_box;
            std::vector<FacePrint> projected_facets;
            Polygon_set projected_surface;

            bool has_same_footprint(BrickPrint const& other) const;
            bool has_same_facets(BrickPrint const& other) const;

            friend std::ostream & operator <<(std::ostream & os, BrickPrint const& brick_projection);
            friend bool operator ==(BrickPrint const& lhs, BrickPrint const& rhs);
            friend bool operator !=(BrickPrint const& lhs, BrickPrint const& rhs);
        };

        BrickPrint & operator +(BrickPrint & lhs, BrickPrint & rhs);
    }
    void swap(projection::BrickPrint & lhs, projection::BrickPrint & rhs);
}
