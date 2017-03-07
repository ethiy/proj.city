#pragma once

#include "../../projection/Raster/raster_projection.h"
#include "../../projection/Brick/brick_projection.h"

#include <vector>

namespace urban
{
    /**
     * Rasterize projection
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @param height the image height
     * @param width the image width
     * @return The image matrix as an array
     */
    std::vector<double> rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, const size_t & height, const size_t & width);

    /**
     * Convert double image to a uint16_t image
     * @param original_image image vector
     * @return target_image image vector in uint16_t
     */
    std::vector<uint16_t> convert(const std::vector<double> & original_image);

    /**
     * Rasterize projection
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @return The image matrix with its geographic reference point
     */
     projection::RasterPrint rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size);
}
