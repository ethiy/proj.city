#pragma once

#include <projection/brick_projection.h>

#include <scene/unode.h>

#include <gdal_priv.h>

namespace urban
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

            double area(void) const;
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

    }
    void swap(projection::FootPrint & lhs, projection::FootPrint & rhs);

    double area(projection::FootPrint const& footprint);
    double circumference(projection::FootPrint const& footprint);
}
