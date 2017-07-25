#pragma once

#include "../scene/scene_projection.h"
#include "../../shadow/Point/point.h"

#include <gdal_priv.h>

#include <vector>
#include <array>

#include <string>
#include <ostream>

namespace urban
{
    namespace projection
    {
        class RasterPrint
        {
        public:
            RasterPrint(void);
            RasterPrint(FootPrint const& footprint, double _pixel_size);
            RasterPrint(std::string const& _name, double const geographic_transform[6], int const& _epsg_index, std::size_t const& _height, std::size_t const& _width, GDALRasterBand* raster_band);
            RasterPrint(RasterPrint const& other);
            RasterPrint(RasterPrint && other);
            ~RasterPrint(void);

            void swap(RasterPrint & other);
            RasterPrint & operator =(RasterPrint const& other) noexcept;
            RasterPrint & operator =(RasterPrint && other) noexcept;

            std::string const& get_name(void) const noexcept;
            shadow::Point const& get_reference_point() const noexcept;
            unsigned short const& get_epsg() const noexcept;
            std::size_t get_height(void) const noexcept;
            std::size_t get_width(void) const noexcept;
            double const& get_pixel_size() const noexcept;

            std::array<double, 6> get_geographic_transform(void) const;

            std::size_t get_index(std::size_t const& i, std::size_t const& j) const noexcept;
            std::vector<short> const& pixel_hits(void) const noexcept;
            double* data(void) noexcept;
            const double* data(void) const noexcept;

            double & at(std::size_t const& i, std::size_t const& j);
            const double & at(std::size_t const& i, std::size_t const& j) const;
            short & hit(std::size_t const& i, std::size_t const& j);
            const short & hit(std::size_t const& i, std::size_t const& j) const;

            typedef std::vector<double>::iterator iterator;
            typedef std::vector<double>::const_iterator const_iterator;

            iterator begin(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            iterator end(void) noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cend(void) const noexcept;

            void horizontal_offset(void);

            RasterPrint & operator +=(RasterPrint const& other);
            RasterPrint & operator -=(RasterPrint const& other);
        private:
            std::string name;
            shadow::Point reference_point;
            unsigned short epsg_index = 2154;
            std::size_t height = 0;
            std::size_t width = 0;
            double pixel_size = .06;
            std::vector<double> image_matrix;
            std::vector<short> pixel_access;
            bool offset = false;

            friend std::ostream & operator <<(std::ostream & os, RasterPrint const& raster_projection);
            friend bool operator ==(RasterPrint const& lhs, RasterPrint const& rhs);
        };

        RasterPrint & operator +(RasterPrint & lhs, RasterPrint const& rhs);
        RasterPrint & operator -(RasterPrint & lhs, RasterPrint const& rhs);
        bool operator !=(RasterPrint & lhs, RasterPrint const& rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);
}
