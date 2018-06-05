#pragma once

#include <projection/brick_projection.h>

#include <scene/unode.h>
#include <scene/scene.h>

#include <gdal_priv.h>

namespace city
{
    namespace projection
    {
        class FootPrint
        {
        public:
            FootPrint(void);
            FootPrint(scene::UNode const& unode);
            FootPrint(std::string const& _name, OGRLayer* projection_layer);
            FootPrint(FootPrint const& other);
            FootPrint(FootPrint && other);
            ~FootPrint(void);

            void swap(FootPrint & other);
            FootPrint & operator =(FootPrint const& other);
            FootPrint & operator =(FootPrint && other);

            std::string const& get_name(void) const noexcept;
            shadow::Point const& get_reference_point(void) const noexcept;
            unsigned short get_epsg(void) const noexcept;
            Bbox_2 const& bbox(void) const noexcept;
            BrickPrint const& data(void) const noexcept;

            inline bool empty(void) const noexcept
            {
                return projection.empty();
            }

            std::vector<double> areas(void) const;
            double area(void) const;
            std::vector<double> edge_lengths(void) const;
            double circumference(void) const;
            
            using iterator = BrickPrint::iterator;
            using const_iterator = BrickPrint::const_iterator;

            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            FootPrint & operator +=(FootPrint const& other);

            void to_ogr(GDALDataset* file, bool labels) const;

            std::vector<double> & rasterize(std::vector<double> & image, shadow::Point const& top_left, std::size_t const height, std::size_t const width, double const pixel_size) const;
        private:
            std::string name;
            shadow::Point reference_point;
            unsigned short epsg_index = 2154;
            BrickPrint projection;
            
            friend std::ostream & operator <<(std::ostream & os, FootPrint const& footprint);
            friend bool operator ==(FootPrint const& lhs, FootPrint const& rhs);
            friend bool operator !=(FootPrint const& lhs, FootPrint const& rhs);
        };
        FootPrint operator +(FootPrint const& lhs, FootPrint const& rhs);

        class ScenePrint
        {
        public:
            ScenePrint(void);
            ScenePrint(scene::Scene const& scene);
            ScenePrint(ScenePrint const& other);
            ScenePrint(ScenePrint && other);
            ~ScenePrint(void);

            void swap(ScenePrint & other);
            ScenePrint & operator =(ScenePrint const& other);
            ScenePrint & operator =(ScenePrint && other);

            shadow::Point const& get_reference_point(void) const noexcept
            {
                return pivot;
            }
            inline unsigned short get_epsg(void) const noexcept
            {
                return epsg_index;
            }
            inline std::vector<FootPrint> const& get_buildings(void) const noexcept
            {
                return buildings;
            }
            inline FootPrint const& get_terrain(void) const noexcept
            {
                return terrain;
            }
            inline bool empty(void) const noexcept
            {
                return buildings.empty();
            }

            Bbox_2 bbox(void) const;

            std::vector<double> areas(void) const;
            std::vector<double> circumferences(void) const;
            
            using iterator = std::vector<FootPrint>::iterator;
            using const_iterator = std::vector<FootPrint>::const_iterator;

            inline iterator begin(void) noexcept
            {
                return buildings.begin();
            }
            inline iterator end(void) noexcept
            {
                return buildings.end();
            }
            inline const_iterator begin(void) const noexcept
            {
                return buildings.begin();
            }
            inline const_iterator end(void) const noexcept
            {
                return buildings.end();
            }
            inline const_iterator cbegin(void) const noexcept
            {
                return buildings.cbegin();
            }
            inline const_iterator cend(void) const noexcept
            {
                return buildings.cend();
            }

            void to_ogr(GDALDataset* file, bool labels) const;

            std::vector<RasterPrint> rasterize(double const pixel_size) const;
        private:
            Point_3 pivot;
            unsigned short epsg_index = 2154;
            std::vector<FootPrint> buildings;
            FootPrint terrain;
            
            friend std::ostream & operator <<(std::ostream & os, ScenePrint const& footprint);
            friend bool operator ==(ScenePrint const& lhs, ScenePrint const& rhs);
            friend bool operator !=(ScenePrint const& lhs, ScenePrint const& rhs);
        };
    }
    void swap(projection::FootPrint & lhs, projection::FootPrint & rhs);

    std::vector<double> areas(projection::FootPrint const& footprint);
    double area(projection::FootPrint const& footprint);
    std::vector<double> edge_lengths(projection::FootPrint const& footprint);
    double circumference(projection::FootPrint const& footprint);
}
