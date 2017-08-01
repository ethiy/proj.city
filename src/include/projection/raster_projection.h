#pragma once

#include <projection/scene_projection.h>
#include <shadow/point.h>

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
            RasterPrint(std::string const& filename, GDALDataset* raster_file);
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

            std::size_t get_index(std::size_t const& i, std::size_t const& j) const noexcept;
            double* data(void) noexcept;
            const double* data(void) const noexcept;

            double & at(std::size_t const& i, std::size_t const& j);
            const double & at(std::size_t const& i, std::size_t const& j) const;
            short & hit(std::size_t const& i, std::size_t const& j);
            const short & hit(std::size_t const& i, std::size_t const& j) const;

            using iterator = std::vector<double>::iterator;
            using const_iterator = std::vector<double>::const_iterator;
            iterator begin(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            iterator end(void) noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cend(void) const noexcept;

            void vertical_offset(void);

            void to_gdal(GDALDataset* file) const;
        private:
            std::string name;
            shadow::Point reference_point;
            unsigned short epsg_index = 2154;
            std::size_t height = 0;
            std::size_t width = 0;
            double pixel_size = .06;
            std::vector<double> image_matrix;
            std::vector<short> pixel_hits;
            bool offset = false;

            friend std::ostream & operator <<(std::ostream & os, RasterPrint const& raster_projection);

            bool equal_metadata(RasterPrint const& other) const;
            friend bool operator ==(RasterPrint const& lhs, RasterPrint const& rhs);

            void set_geotransform(GDALDataset* file) const;
            void set_projection(GDALDataset* file) const;
            void save_image(GDALDataset* file) const;
        };

        bool operator !=(RasterPrint & lhs, RasterPrint const& rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);
}
