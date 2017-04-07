#pragma once

#include "../../geometry_definitions.h"
#include "../../shadow/Point/point.h"

#include <ogrsf_frmts.h>

#include <vector>
#include <utility>

#include <ostream>

namespace urban
{
    namespace projection
    {
        class RasterPrint;
        
        class FacePrint
        {
        public:
            FacePrint(void);
            FacePrint(const Polygon_with_holes & _border, const Plane_3 & _supporting_plane);
            FacePrint(OGRFeature* ogr_facet, OGRFeatureDefn* facet_definition);
            FacePrint(const FacePrint & other);
            FacePrint(FacePrint && other);
            ~FacePrint(void);

            void swap(FacePrint & other);

            FacePrint & operator=(const FacePrint & other);

            FacePrint & operator=(FacePrint && other);

            Polygon_with_holes get_polygon(void) const noexcept;
            Polygon outer_boundary(void) const;
            Plane_3 get_plane(void) const noexcept;
            Vector_3 get_normal(void) const noexcept;

            double get_plane_height(const Point_2 &) const;
            double get_plane_height(const InexactPoint_2 & inexact_point) const;
            double get_height(const Point_2 & point) const;
            double get_height(const InexactPoint_2 & inexact_point) const;
            double get_height(double top_left_x, double top_left_y, double pixel_size, bool & hit) const;
            double area(void) const;
            Bbox_2 bbox(void) const;

            RasterPrint & rasterize_to(RasterPrint & raster_projection, const shadow::Point & pivot, std::vector<short> & pixel_access) const;

            bool has_same_border(const FacePrint & other) const;
            bool has_same_plane(const FacePrint & other) const;

            typedef Polygon_with_holes::Hole_const_iterator Hole_const_iterator;
            Hole_const_iterator holes_begin(void) const;
            Hole_const_iterator holes_end(void) const;
            
            /**
            * Checks if the face projection is degenerate
            * In our case, two edges are coinciding means that all edges are so,
            * Due to the fact that all edges are supported by the same plane:
            *  - This means that we should just check if the area is null to assert
            *      that the projection is degenarate.
            * @return boolean: true if degenerate and false if not
            */
            bool is_degenerate(void) const;
            bool is_perpendicular(void) const;

            bool contains(const Point_2 & point) const;
            bool contains(const InexactPoint_2 & inexact_point) const;

            OGRFeature* to_ogr(OGRFeatureDefn* feature_definition) const;
        private:
            Polygon_with_holes border;
            Plane_3 supporting_plane;

            friend std::ostream & operator<<(std::ostream & os, const FacePrint & facet);
        };
        
        bool operator==(const FacePrint & lhs, const FacePrint & rhs);
        bool operator!=(const FacePrint & lhs, const FacePrint & rhs);
    }

    void swap(projection::FacePrint & lhs, projection::FacePrint & rhs);
    
    /** Compute FacePrint area*/
    double area(const projection::FacePrint & facet);
}
