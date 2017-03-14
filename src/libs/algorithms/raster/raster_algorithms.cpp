#include "raster_algorithms.h"

#include "../../shadow/Point/point.h"

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    std::vector<double> rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, const size_t & height, const size_t & width)
    {
        
        std::vector<double> pixels(width * height);
        std::vector<size_t> indexes(width * height);
        std::iota(std::begin(indexes), std::end(indexes), 0);
        InexactToExact to_exact;
        std::transform(
            std::begin(indexes),
            std::end(indexes),
            std::begin(pixels),
            [&brick_projection, pixel_size, height, width, &to_exact](const size_t index)
            {
                return brick_projection.get_height(
                    Point_2(
                        to_exact(brick_projection.bbox().xmin() + (static_cast<double>(index%height) + .5) * pixel_size),
                        to_exact(brick_projection.bbox().ymin() + (static_cast<double>(index/height) + .5) * pixel_size)
                    )
                );
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

    projection::RasterPrint rasterize(const projection::BrickPrint & brick_projection, const double & pixel_size, double maximum_value)
    {
        size_t height = std::ceil((brick_projection.bbox().xmax() - brick_projection.bbox().xmin()) / pixel_size);
        size_t width = std::ceil((brick_projection.bbox().ymax() - brick_projection.bbox().ymin()) / pixel_size);

        return projection::RasterPrint(
            brick_projection.get_name(),
            shadow::Point(brick_projection.bbox().xmin(), brick_projection.bbox().ymin(), 0),
            height,
            width,
            pixel_size,
            rasterize(
                brick_projection,
                pixel_size,
                height,
                width
            )
        );

    }
}
