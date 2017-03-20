#include "raster_projection.h"

#include <Imagine/LinAlg.h>

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    namespace projection
    {
        RasterPrint::RasterPrint(void) {}
        RasterPrint::RasterPrint(const std::string & _name, const shadow::Point & _reference_point, const size_t _height, const size_t _width, double _pixel_size, const std::vector<double> & image_array)
            : name(_name), reference_point(_reference_point), height(_height), width(_width), pixel_size(_pixel_size)
        {
            if(image_array.size() != _width * _height)
                throw std::logic_error("the image array should contain exaclty width * height elements");

            double* buffer = new double[width * height];
            std::copy(
                std::begin(image_array),
                std::end(image_array),
                buffer
            );
            image_matrix = Imagine::Image<double>(buffer, width, height);
        }

        RasterPrint::RasterPrint(const std::string & _name, const shadow::Point & _reference_point, const size_t _height, const size_t _width, double _pixel_size)
            : name(_name), reference_point(_reference_point), height(_height), width(_width), pixel_size(_pixel_size), image_matrix(Imagine::Image<double>(width, height)) {}

        RasterPrint::RasterPrint(const std::string & _name, const double geographic_transform[6], const size_t _height, const size_t _width, GDALRasterBand* raster_band): name(_name), reference_point(shadow::Point(geographic_transform[0], geographic_transform[3], 0)), height(_width), width(_height), pixel_size(geographic_transform[1])
        {
            if(std::abs(geographic_transform[1] - geographic_transform[5]) > std::numeric_limits<double>::epsilon())
                throw std::logic_error("this case is not treated here");
            
            double* buffer = new double[width * height];
            CPLErr error = raster_band->RasterIO(GF_Read, 0, 0, _width, _height, buffer, _width, _height, GDT_Float64,0, 0);
            if(error != CE_None)
                throw std::runtime_error("GDAL could not read raster band");
            image_matrix = Imagine::Image<double>(buffer, width, height);
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

        shadow::Point RasterPrint::get_reference_point() const noexcept
        {
            return reference_point;
        }

        double RasterPrint::get_pixel_size() const noexcept
        {
            return pixel_size;
        }

        std::array<double, 6> RasterPrint::get_geographic_transform(void) const
        {
            return std::array<double, 6>{{reference_point.x(), pixel_size, 0, reference_point.y(), 0, pixel_size}};
        }

        double* RasterPrint::data(void) const
        {
            double* raster_array = reinterpret_cast<double*>(calloc(sizeof(double), width * height));
            std::transform(
                image_matrix.coordsBegin(),
                image_matrix.coordsEnd(),
                raster_array,
                [this](const Imagine::Coords<2> & coordinates)
                {
                    return image_matrix(coordinates);
                }
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

        double & RasterPrint::at(const size_t i, const size_t j)
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return image_matrix(i, j);
        }

        const double & RasterPrint::at(const size_t i, const size_t j) const
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return image_matrix(i, j);
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

        // RasterPrint & RasterPrint::rescale(const double & _pixel_size)
        // {
        //     cv::resize(image_matrix, image_matrix, cv::Size(), pixel_size / _pixel_size, cv::INTER_LINEAR);
        //     pixel_size = _pixel_size;
        //     height = image_matrix.size().height;
        //     width = image_matrix.size().width;
        //     return *this;
        // }

        RasterPrint & RasterPrint::operator+=(const RasterPrint & other)
        {
            if(std::abs(pixel_size - other.pixel_size) > std::numeric_limits<double>::epsilon() && reference_point != other.reference_point)
                throw std::logic_error("Case not treated");
            image_matrix += other.image_matrix;
            return *this;
        }
        
        RasterPrint & RasterPrint::operator-=(const RasterPrint & other)
        {
            if(std::abs(pixel_size - other.pixel_size) > std::numeric_limits<double>::epsilon() && reference_point != other.reference_point)
                throw std::logic_error("Case not treated");
            image_matrix -= other.image_matrix;
            return *this;
        }

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

        RasterPrint & operator+(RasterPrint & lhs, const RasterPrint & rhs)
        {
            return lhs += rhs;
        }

        RasterPrint & operator-(RasterPrint & lhs, const RasterPrint & rhs)
        {
            return lhs -= rhs;
        }

        bool operator==(RasterPrint & lhs, const RasterPrint & rhs)
        {
            return std::abs(lhs.pixel_size - rhs.pixel_size) < std::numeric_limits<double>::epsilon() && lhs.reference_point == rhs.reference_point && lhs.image_matrix == rhs.image_matrix;
        }
        
        bool operator!=(RasterPrint & lhs, const RasterPrint & rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
