#include "raster_algorithms.h"

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    std::vector<double> rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, const size_t & width, const size_t & height, double z_offset)
    {
        
        std::vector<double> pixels(width * height);
        std::vector<size_t> indexes(width * height);
        std::iota(std::begin(indexes), std::end(indexes), 0);
        std::transform(
            std::begin(indexes),
            std::end(indexes),
            std::begin(pixels),
            [&brick_projection, pixel_size, height, width, z_offset](const size_t index)
            {
                return brick_projection.get_height(
                    Point_2(
                        brick_projection.bbox().xmin() + (static_cast<double>(index%width) + .5) * pixel_size,
                        brick_projection.bbox().ymin() + (static_cast<double>(index/width) + .5) * pixel_size
                    )
                ) + z_offset;
            }
        );
        return pixels;
    }

    std::vector<uint16_t> convert(const std::vector<double> & original_image, double maximum_value)
    {
        unsigned long long dynamic = std::pow(2, sizeof(uint16_t) * 8) - 1;
        std::vector<uint16_t> target_image(original_image.size());
        std::transform(
            std::begin(original_image),
            std::end(original_image),
            std::begin(target_image),
            [dynamic, maximum_value](const double value)
            {
                return static_cast<uint16_t>( dynamic * value / maximum_value);
            }
        );
        return target_image;
    }

    projection::RasterPrint rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, shadow::Point pivot)
    {
        size_t width = std::ceil((brick_projection.bbox().xmax() - brick_projection.bbox().xmin()) / pixel_size);
        size_t height = std::ceil((brick_projection.bbox().ymax() - brick_projection.bbox().ymin()) / pixel_size);

        projection::RasterPrint rasta(
            brick_projection.get_name(),
            shadow::Point(pivot.x() + brick_projection.bbox().xmin(), pivot.y() + brick_projection.bbox().ymin(), pivot.z()),
            height,
            width,
            pixel_size
        );

        std::accumulate(
            brick_projection.cbegin(),
            brick_projection.cend(),
            projection::RasterPrint(
                brick_projection.get_name(),
                shadow::Point(pivot.x() + brick_projection.bbox().xmin(), pivot.y() + brick_projection.bbox().ymin(), pivot.z()),
                height,
                width,
                pixel_size
            ),
            [](projection::RasterPrint result, const projection::FacePrint & face_projection)
            {
                return face_projection.rasterize_to(result);
            }
        );

        return rasta;
    }
}
