#pragma once

#include "../../algorithms/projection/projection_algorithms.h"
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
            RasterPrint(std::string const& _name, shadow::Point const& _reference_point, size_t const _height, size_t const _width, double _pixel_size, std::vector<double> const& image_array);
            RasterPrint(std::string const& _name, shadow::Point const& _reference_point, size_t const _height, size_t const _width, double _pixel_size);
            RasterPrint(std::string const& _name, double const geographic_transform[6], size_t const _height, size_t const _width, GDALRasterBand* raster_band);
            RasterPrint(RasterPrint const& other);
            RasterPrint(RasterPrint && other);
            ~RasterPrint(void);

            std::string get_name(void) const noexcept;
            size_t get_height(void) const noexcept;
            size_t get_width(void) const noexcept;
            size_t get_index(const size_t i, const size_t j) const noexcept;
            shadow::Point get_reference_point() const noexcept;
            double get_pixel_size() const noexcept;
            std::array<double, 6> get_geographic_transform(void) const;
            std::vector<short> const& pixel_hits(void) const noexcept;
            double* data(void) noexcept;
            const double* data(void) const noexcept;

            void swap(RasterPrint & other);

            double & at(const size_t i, const size_t j);
            const double & at(const size_t i, const size_t j) const;
            short & hit(const size_t i, const size_t j);
            const short & hit(const size_t i, const size_t j) const;

            typedef std::vector<double>::iterator iterator;
            typedef std::vector<double>::const_iterator const_iterator;

            iterator begin(void) noexcept;
            const_iterator cbegin(void) const noexcept;
            iterator end(void) noexcept;
            const_iterator cend(void) const noexcept;

            RasterPrint & operator=(RasterPrint const& other) noexcept;
            RasterPrint & operator=(RasterPrint && other) noexcept;

            RasterPrint & operator+=(RasterPrint const& other);
            RasterPrint & operator-=(RasterPrint const& other);
        private:
            std::string name;
            shadow::Point reference_point;
            size_t height = 0;
            size_t width = 0;
            double pixel_size = 0.6;
            std::vector<double> image_matrix;
            std::vector<short> pixel_access;

            friend std::ostream & operator<<(std::ostream & os, RasterPrint const& raster_projection);
            friend bool operator==(RasterPrint const& lhs, RasterPrint const& rhs);
        };

        RasterPrint & operator+(RasterPrint & lhs, RasterPrint const& rhs);
        RasterPrint & operator-(RasterPrint & lhs, RasterPrint const& rhs);
        bool operator!=(RasterPrint & lhs, RasterPrint const& rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);

    
}
