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
     * @param pivot the pivot point (generally the pointwise scene mean)
     * @return The image matrix with its geographic reference point
     */
    projection::RasterPrint rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, const shadow::Point & pivot);
}
