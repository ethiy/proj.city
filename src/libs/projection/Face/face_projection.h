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
            FacePrint(const Polygon_with_holes & _border, Plane_3 const& _supporting_plane);
            FacePrint(OGRFeature* ogr_facet, OGRFeatureDefn* facet_definition);
            FacePrint(FacePrint const& other);
            FacePrint(FacePrint && other);
            ~FacePrint(void);

            void swap(FacePrint & other);

            FacePrint & operator =(FacePrint const& other) noexcept;
            FacePrint & operator =(FacePrint && other) noexcept;

            Polygon_with_holes get_polygon(void) const noexcept;
            Polygon outer_boundary(void) const;
            Plane_3 get_plane(void) const noexcept;
            Vector_3 get_normal(void) const noexcept;

            double get_plane_height(Point_2 const&) const;
            double get_plane_height(InexactPoint_2 const& inexact_point) const;
            double get_height(Point_2 const& point) const;
            double get_height(InexactPoint_2 const& inexact_point) const;
            double get_height(double top_left_x, double top_left_y, double pixel_size, bool & hit) const;

            InexactPoint_2 centroid(void) const;
            double area(void) const;
            Bbox_2 bbox(void) const;

            RasterPrint & rasterize_to(RasterPrint & raster_projection, const shadow::Point & pivot) const;

            bool has_same_border(FacePrint const& other) const;
            bool has_same_plane(FacePrint const& other) const;

            typedef Polygon_with_holes::Hole_const_iterator Hole_const_iterator;
            Hole_const_iterator holes_begin(void) const;
            Hole_const_iterator holes_end(void) const;
            
            /**
            * Checks if the face projection is degenerate
            * In our case, two edges are coinciding means that all edges are so,
            * Due to the fact that all edges are supported by the same plane:
            *  - This means that we should just check if the area is null to assert
            *      that the projection is degenerate.
            * @return boolean: true if degenerate and false if not
            */
            bool is_degenerate(void) const;
            bool is_perpendicular(void) const;
            bool is_empty(void) const;

            bool contains(Point_2 const& point) const;
            bool contains(InexactPoint_2 const& inexact_point) const;

            OGRFeature* to_ogr(OGRFeatureDefn* feature_definition, shadow::Point const& reference_point) const;
        private:
            Polygon_with_holes border;
            Plane_3 supporting_plane;

            friend std::ostream & operator <<(std::ostream & os, FacePrint const& facet);
        };
        
        bool operator ==(FacePrint const& lhs, FacePrint const& rhs);
        bool operator !=(FacePrint const& lhs, FacePrint const& rhs);
    }
    void swap(projection::FacePrint & lhs, projection::FacePrint & rhs);
    
    /**
     * Compute FacePrint area.
     * @param facet a facet projection
     * @return area of the facet projection
     */
    double area(const projection::FacePrint & facet);
}
