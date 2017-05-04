#pragma once

#include "../../projection/Raster/raster_projection.h"
#include "../../projection/Brick/brick_projection.h"
#include "../../shadow/Geometry/point.h"

#include <vector>

namespace urban
{
    /**
     * Rasterize projection
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @param pivot the pivot point (generally the pointwise scene mean)
     * @param epsg_code the epsg index for the projection system
     * @return The image matrix with its geographic reference point
     */
    projection::RasterPrint rasterize(projection::BrickPrint const& brick_projection, double const& pixel_size, shadow::Point const& pivot, unsigned short const& epsg_code = 2154);
}
