#pragma once

#include "../Brick/brick_projection.h"
#include "../../algorithms/projection/projection_algorithms.h"
#include "../../shadow/Point/point.h"

#include <opencv2/core.hpp>

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
            RasterPrint(const std::string & _name, shadow::Point _reference_point, size_t height, size_t width, double pixel_size, const std::vector<uint16_t> & image_array);
            RasterPrint(const std::string & _name, const double geographic_transform[6], size_t height, size_t width, GDALRasterBand* raster_band);
            RasterPrint(const RasterPrint & other);
            RasterPrint(RasterPrint && other);
            ~RasterPrint(void);

            size_t get_height(void) const noexcept;
            size_t get_width(void) const noexcept;
            std::array<double, 6> get_geographic_transform(void) const;
            std::vector<GUInt16> get_array(void) const;

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
            cv::Mat image_matrix;

            friend std::ostream & operator<<(std::ostream & os, const RasterPrint & raster_projection);
        };

        RasterPrint & operator+(const RasterPrint & lhs, const RasterPrint & rhs);
        RasterPrint & operator-(const RasterPrint & lhs, const RasterPrint & rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);
}
