#pragma once

#include <geometry_definitions.h>
#include <scene/unode.h>
#include <shadow/point.h>

#include <ogrsf_frmts.h>

#include <vector>
#include <utility>

#include <ostream>

namespace city
{
    namespace projection
    {
        class RasterPrint;
        
        class FacePrint
        {
        public:
            FacePrint(void);
            FacePrint(std::size_t const _id, Polygon_with_holes const& _border, Plane_3 const& _supporting_plane);
            FacePrint(::city::scene::UNode const& unode, ::city::scene::UNode::Facet const& facet);
            FacePrint(OGRFeature* ogr_facet, OGRFeatureDefn* facet_definition);
            FacePrint(FacePrint const& other);
            FacePrint(FacePrint && other);
            ~FacePrint(void);

            void swap(FacePrint & other);

            FacePrint & operator =(FacePrint const& other) noexcept;
            FacePrint & operator =(FacePrint && other) noexcept;

            std::size_t get_id() const noexcept;
            Polygon_with_holes const& get_polygon(void) const noexcept;
            Polygon const& outer_boundary(void) const;
            Plane_3 const& get_plane(void) const noexcept;
            Vector_3 get_normal(void) const noexcept;
            Bbox_2 bbox(void) const;

            CGAL::Orientation orientation(void) const;

            double get_plane_height(Point_2 const&) const;
            double get_plane_height(InexactPoint_2 const& inexact_point) const;
            double get_height(Point_2 const& point) const;
            double get_height(InexactPoint_2 const& inexact_point) const;
            
            std::vector<Polygon_with_holes> pixel_intersection(double const top_left_x, double const top_left_y, double const pixel_size, bool & hit) const;
            double get_height(double top_left_x, double top_left_y, double pixel_size, bool & hit) const;

            InexactPoint_2 centroid(void) const;
            double area(void) const;
            double circumference(void) const;
            
            bool equal_border(FacePrint const& other) const;
            bool equal_plane(FacePrint const& other) const;

            typedef Polygon_with_holes::Hole_const_iterator Hole_const_iterator;
            Hole_const_iterator holes_begin(void) const;
            Hole_const_iterator holes_end(void) const;
            
            /**
            * Checks if the face projection is degenerate
            * In our case, two edges are coinciding means that all edges are so,
            * Due to the fact that all edges are supported by the same plane:
            *  - This means that we should just check if the area is nullptr to assert
            *      that the projection is degenerate.
            * @return boolean: true if degenerate and false if not
            */
            bool is_degenerate(void) const;
            bool is_perpendicular(void) const;
            bool empty(void) const;

            bool contains(Point_2 const& point) const;
            bool contains(InexactPoint_2 const& inexact_point) const;

            OGRFeature* to_ogr(OGRFeatureDefn* feature_definition, shadow::Point const& reference_point, bool labels) const;
            
            std::vector<double> & rasterize(std::vector<double> & image, std::vector<short> & hits, shadow::Point const& top_left, std::size_t const height, std::size_t const width, double const pixel_size) const;
        private:
            std::size_t id;
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
    double area(projection::FacePrint const& facet);
    
    /**
     * Compute FacePrint circumference.
     * @param facet a facet projection
     * @return circumference of the facet projection
     */
    double circumference(projection::FacePrint const& facet);
}
