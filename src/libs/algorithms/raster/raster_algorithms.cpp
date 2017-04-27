#include "raster_algorithms.h"

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    projection::RasterPrint rasterize(projection::BrickPrint const& brick_projection, double const& pixel_size, shadow::Point const& pivot, unsigned short const& epsg_code)
    {
        std::size_t h = static_cast<std::size_t>(std::ceil((brick_projection.bbox().ymax() - brick_projection.bbox().ymin()) / pixel_size));
        std::size_t w = static_cast<std::size_t>(std::ceil((brick_projection.bbox().xmax() - brick_projection.bbox().xmin()) / pixel_size));
        
        projection::RasterPrint raster_image = std::accumulate(
            brick_projection.cbegin(),
            brick_projection.cend(),
            projection::RasterPrint(
                brick_projection.get_name(),
                shadow::Point(pivot.x() + brick_projection.bbox().xmin(), pivot.y() + brick_projection.bbox().ymax(), pivot.z()),
                epsg_code,
                h,
                w,
                pixel_size
            ),
            [&pivot](projection::RasterPrint result, projection::FacePrint const& face_projection)
            {
                return face_projection.rasterize_to(result, pivot);
            }
        );

        raster_image.horizontal_offset();
        
        return raster_image;
    }
}
