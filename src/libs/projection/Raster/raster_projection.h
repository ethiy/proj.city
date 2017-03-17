#pragma once

#include "../Brick/brick_projection.h"
#include "../../algorithms/projection/projection_algorithms.h"
#include "../../shadow/Point/point.h"

#include <Imagine/Images.h>

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
            RasterPrint(const std::string & _name, const shadow::Point & _reference_point, const size_t height, const size_t width, double pixel_size, const std::vector<double> & image_array);
            RasterPrint(const std::string & _name, const double geographic_transform[6], const size_t height, const size_t width, GDALRasterBand* raster_band);
            RasterPrint(const RasterPrint & other);
            RasterPrint(RasterPrint && other);
            ~RasterPrint(void);

            size_t get_height(void) const noexcept;
            size_t get_width(void) const noexcept;
            shadow::Point get_reference_point() const noexcept;
            double get_pixel_size() const noexcept;
            std::array<double, 6> get_geographic_transform(void) const;
            double* data(void) const;

            void swap(RasterPrint & other);

            RasterPrint & operator=(const RasterPrint & other) noexcept;
            RasterPrint & operator=(RasterPrint && other) noexcept;

            RasterPrint & rescale(const double & _pixel_size);

            RasterPrint & operator+=(const RasterPrint & other);
            RasterPrint & operator-=(const RasterPrint & other);
        private:
            std::string name;
            shadow::Point reference_point;
            size_t height;
            size_t width;
            double pixel_size;
            Imagine::Image<double> image_matrix;

            friend std::ostream & operator<<(std::ostream & os, const RasterPrint & raster_projection);
            friend bool operator==(RasterPrint & lhs, const RasterPrint & rhs);
        };

        RasterPrint & operator+(RasterPrint & lhs, const RasterPrint & rhs);
        RasterPrint & operator-(RasterPrint & lhs, const RasterPrint & rhs);
        bool operator!=(RasterPrint & lhs, const RasterPrint & rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);

    
}
