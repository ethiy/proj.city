#include "raster_algorithms.h"

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    projection::RasterPrint rasterize(projection::BrickPrint const& brick_projection, double const& pixel_size, shadow::Point const& pivot)
    {
        size_t h = static_cast<size_t>(std::ceil((brick_projection.bbox().ymax() - brick_projection.bbox().ymin()) / pixel_size));
        size_t w = static_cast<size_t>(std::ceil((brick_projection.bbox().xmax() - brick_projection.bbox().xmin()) / pixel_size));
        return std::accumulate(
            brick_projection.cbegin(),
            brick_projection.cend(),
            projection::RasterPrint(
                brick_projection.get_name(),
                shadow::Point(pivot.x() + brick_projection.bbox().xmin(), pivot.y() + brick_projection.bbox().ymax(), pivot.z()),
                h,
                w,
                pixel_size
            ),
            [&pivot](projection::RasterPrint result, const projection::FacePrint & face_projection)
            {
                return face_projection.rasterize_to(result, pivot);
            }
        );
    }
}
