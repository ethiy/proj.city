#pragma once

#include "../../projection/Raster/raster_projection.h"
#include "../../projection/Brick/brick_projection.h"
#include "../../shadow/Point/point.h"

#include <vector>

namespace urban
{
    /**
     * Rasterize projection
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @param height the image height
     * @param width the image width
     * @param z_offset the z offset
     * @return The image matrix as an array
     */
    std::vector<double> rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, const size_t & height, const size_t & width, double z_offset);

    /**
     * Convert double image to a uint16_t image
     * @param original_image image vector
     * @param maximum_value the maximal height
     * @return target_image image vector in uint16_t
     */
    std::vector<uint16_t> convert(const std::vector<double> & original_image, double maximum_value);

    /**
     * Rasterize projection
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @param pivot the pivot point (generally the pointwise scene mean)
     * @return The image matrix with its geographic reference point
     */
    projection::RasterPrint rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, shadow::Point pivot);
}
