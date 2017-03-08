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
        RasterPrint::RasterPrint(const std::string & _name, shadow::Point _reference_point, size_t _height, size_t _width, double _pixel_size, const std::vector<uint16_t> & image_array)
            : name(_name), reference_point(_reference_point), height(_height), width(_width), pixel_size(_pixel_size), image_matrix(image_array)
        {
            if(image_array.size() != _width * _height)
                throw std::logic_error("the image array should contain exaclty width * height elements");
            image_matrix.reshape(_width);
        }

        RasterPrint::RasterPrint(const std::string & _name, const double geographic_transform[6], size_t _height, size_t _width, GDALRasterBand* raster_band): name(_name), reference_point(shadow::Point(geographic_transform[0], geographic_transform[3], 0)), height(_height), width(_width), pixel_size(geographic_transform[1])
        {
            if(geographic_transform[1] != geographic_transform[5])
                throw std::logic_error("this case is not treated here");
            
            GUInt16* buffer = new GUInt16[width * height];
            CPLErr error = raster_band->RasterIO(GF_Read, 0, 0, width, height, buffer, width, height, GDT_UInt16,0, 0);
            if(error != CE_None)
                throw std::runtime_error("GDAL could not read raster band");
            image_matrix = cv::Mat(height, width, CV_16UC1, buffer);
            std::free(buffer);
        }

        RasterPrint::RasterPrint(const RasterPrint & other)
            : name(other.name), reference_point(other.reference_point), height(other.height), width(other.width), pixel_size(other.pixel_size), image_matrix(other.image_matrix) {}
        RasterPrint::RasterPrint(RasterPrint && other)
            : name(std::move(other.name)), reference_point(std::move(other.reference_point)), height(std::move(other.height)), width(std::move(other.width)), pixel_size(std::move(other.pixel_size)), image_matrix(std::move(other.image_matrix)) {}
        RasterPrint::~RasterPrint(void) {}


        size_t RasterPrint::get_height(void) const noexcept
        {
            return height;
        }

        size_t RasterPrint::get_width(void) const noexcept
        {
            return width;
        }

        std::array<double, 6> RasterPrint::get_geographic_transform(void) const
        {
            return std::array<double, 6>{{reference_point.x(), pixel_size, 0, reference_point.y(), 0, pixel_size}};
        }

        std::vector<GUInt16> RasterPrint::get_array(void) const
        {
            std::vector<GUInt16> raster_array(width * height);
            std::copy(
                image_matrix.begin<GUInt16>(),
                image_matrix.end<GUInt16>(),
                std::begin(raster_array)
            );
            return raster_array;
        }


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

        RasterPrint & RasterPrint::operator+=(const RasterPrint & other)
        {

        }
        
        RasterPrint & RasterPrint::operator-=(const RasterPrint & other);

        std::ostream & operator<<(std::ostream & os, const RasterPrint & raster_projection)
        {
            os << "Name : " << raster_projection.name << std::endl
               << "Reference (Top left) Point : " << raster_projection.reference_point << std::endl
               << "Height : " << raster_projection.height << std::endl
               << "Width : " << raster_projection.width << std::endl
               << "Image Matrix : " << std::endl
               << raster_projection.image_matrix << std::endl;
               
            return os;
        }
    }

    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
