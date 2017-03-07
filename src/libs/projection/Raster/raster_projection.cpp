#include "raster_projection.h"

#include <opencv2/imgproc.hpp>

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    namespace projection
    {
        RasterPrint::RasterPrint(void) {}
        RasterPrint::RasterPrint(const RasterPrint & other): name(other.name), reference_point(other.reference_point), height(other.height), width(other.width), pixel_size(other.pixel_size), image_matrix(other.image_matrix) {}
        RasterPrint::RasterPrint(RasterPrint && other): name(std::move(other.name)), reference_point(std::move(other.reference_point)), height(std::move(other.height)), width(std::move(other.width)), pixel_size(std::move(other.pixel_size)), image_matrix(std::move(other.image_matrix)) {}
        RasterPrint::~RasterPrint(void) {}

        void RasterPrint::swap(RasterPrint & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(height, other.height);
            swap(width, other.width);
            swap(pixel_size, other.pixel_size);
            swap(image_matrix, other.image_matrix);
        }

        RasterPrint & RasterPrint::operator=(const RasterPrint & other) noexcept
        {
            name = other.name;
            reference_point = other.reference_point;
            height = other.height;
            width = other.width;
            pixel_size = other.pixel_size;
            image_matrix = other.image_matrix;

            return *this;
        }

        RasterPrint & RasterPrint::operator=(RasterPrint && other) noexcept
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            height = std::move(other.height);
            width = std::move(other.width);
            pixel_size = std::move(other.pixel_size);
            image_matrix = std::move(other.image_matrix);
            
            return *this;
        }

        RasterPrint & RasterPrint::rescale(const double & _pixel_size)
        {
            cv::resize(image_matrix, image_matrix, cv::Size(), pixel_size / _pixel_size, cv::INTER_LINEAR);
            pixel_size = _pixel_size;
            height = image_matrix.size().height;
            width = image_matrix.size().width;
            return *this;
        }

        GUInt16* RasterPrint::to_gdal(double reference[6], size_t & height, size_t & width) const
        {
            gdal_reference_init(reference);
            GUInt16* image_array = new GUInt16[height * width];
            std::copy(
                image_matrix.begin<GUInt16>(),
                image_matrix.end<GUInt16>(),
                image_array
            );
            return image_array;
        }

        void RasterPrint::gdal_reference_init(double reference[6]) const
        {
            reference[0] = reference_point.x();
            reference[1] = pixel_size;
            reference[2] = 0;
            reference[3] = reference_point.y();
            reference[4] = pixel_size;
            reference[5] = 0;            
        }
    }

    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
