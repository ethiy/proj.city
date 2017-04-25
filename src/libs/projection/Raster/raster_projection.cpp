#include "raster_projection.h"

#include <iterator>
#include <algorithm>
#include <iomanip>

#include <cmath>

namespace urban
{
    namespace projection
    {
        RasterPrint::RasterPrint(void) {}
        RasterPrint::RasterPrint(std::string const& _name, shadow::Point const& _reference_point, unsigned short const& _epsg_index, size_t const& _height, size_t const& _width, double const& _pixel_size)
            : name(_name),
              reference_point(_reference_point),
              epsg_index(_epsg_index),
              height(_height),
              width(_width),
              pixel_size(_pixel_size),
              image_matrix(_height * _width, 0.),
              pixel_access(_height * _width, 0)
        {}
        RasterPrint::RasterPrint(const std::string & _name, const double geographic_transform[6], int const& _epsg_index, size_t const& _height, size_t const& _width, GDALRasterBand* raster_band)
            : name(_name),
              reference_point(shadow::Point(geographic_transform[0], geographic_transform[3], 0)),
              height(_height),
              width(_width),
              pixel_size(geographic_transform[1]),
              image_matrix(_height * _width, 0.),
              pixel_access(_height * _width, 1)
        {
            if(std::abs(geographic_transform[1] + geographic_transform[5]) > std::numeric_limits<double>::epsilon())
                throw std::logic_error("this case is not treated here");
            
            epsg_index = _epsg_index > 0 ? static_cast<unsigned short>(_epsg_index) : 2154;
            
            double* buffer = reinterpret_cast<double*>(calloc(sizeof(double), _width * _height));
            CPLErr error = raster_band->RasterIO(GF_Read, 0, 0, _width, _height, buffer, _width, _height, GDT_Float64, 0, 0);
            if(error != CE_None)
                throw std::runtime_error("GDAL could not read raster band");
            std::copy(
                buffer,
                buffer + _width * _height,
                std::begin(image_matrix)
            );
        }
        RasterPrint::RasterPrint(RasterPrint const& other)
            : name(other.name),
              reference_point(other.reference_point),
              epsg_index(other.epsg_index),
              height(other.height),
              width(other.width),
              pixel_size(other.pixel_size),
              image_matrix(other.image_matrix),
              pixel_access(other.pixel_access)
        {}
        RasterPrint::RasterPrint(RasterPrint && other)
            : name(std::move(other.name)),
              reference_point(std::move(other.reference_point)),
              epsg_index(std::move(other.epsg_index)),
              height(std::move(other.height)),
              width(std::move(other.width)),
              pixel_size(std::move(other.pixel_size)),
              image_matrix(std::move(other.image_matrix)),
              pixel_access(std::move(other.pixel_access))
        {}
        RasterPrint::~RasterPrint(void) {}


        std::string const& RasterPrint::get_name(void) const noexcept
        {
            return name;
        }

        size_t const& RasterPrint::get_height(void) const noexcept
        {
            return height;
        }

        size_t const& RasterPrint::get_width(void) const noexcept
        {
            return width;
        }

        size_t RasterPrint::get_index(size_t const& i, size_t const& j) const noexcept
        {
            return i * width + j;
        }

        shadow::Point const& RasterPrint::get_reference_point() const noexcept
        {
            return reference_point;
        }

        double const& RasterPrint::get_pixel_size() const noexcept
        {
            return pixel_size;
        }

        std::array<double, 6> RasterPrint::get_geographic_transform(void) const
        {
            return std::array<double, 6>{{reference_point.x(), pixel_size, 0, reference_point.y(), 0, - pixel_size}};
        }

        double* RasterPrint::data(void) noexcept
        {
            return image_matrix.data();
        }
        double const* RasterPrint::data(void) const noexcept
        {
            return image_matrix.data();
        }
        std::vector<short> const& RasterPrint::pixel_hits(void) const noexcept
        {
            return pixel_access;
        }

        void RasterPrint::swap(RasterPrint & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(epsg_index, other.epsg_index);
            swap(height, other.height);
            swap(width, other.width);
            swap(pixel_size, other.pixel_size);
            swap(image_matrix, other.image_matrix);
            swap(pixel_access, other.pixel_access);
        }

        double & RasterPrint::at(size_t const& i, size_t const& j)
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return image_matrix.at(i * width + j);
        }
        double const& RasterPrint::at(size_t const& i, size_t const& j) const
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return image_matrix.at(i * width + j);
        }
        short & RasterPrint::hit(size_t const& i, size_t const& j)
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return pixel_access.at(i * width + j);
        }
        const short & RasterPrint::hit(size_t const& i, size_t const& j) const
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return pixel_access.at(i * width + j);
        }

        RasterPrint::iterator RasterPrint::begin(void) noexcept
        {
            return image_matrix.begin();
        }
        RasterPrint::const_iterator RasterPrint::cbegin(void) const noexcept
        {
            return image_matrix.cbegin();
        }
        RasterPrint::const_iterator RasterPrint::begin(void) const noexcept
        {
            return image_matrix.begin();
        }
        RasterPrint::iterator RasterPrint::end(void) noexcept
        {
            return image_matrix.end();
        }
        RasterPrint::const_iterator RasterPrint::end(void) const noexcept
        {
            return image_matrix.end();
        }
        RasterPrint::const_iterator RasterPrint::cend(void) const noexcept
        {
            return image_matrix.cend();
        }

        RasterPrint & RasterPrint::operator=(RasterPrint const& other) noexcept
        {
            name = other.name;
            reference_point = other.reference_point;
            epsg_index = other.epsg_index;
            height = other.height;
            width = other.width;
            pixel_size = other.pixel_size;
            image_matrix = other.image_matrix;
            pixel_access = other.pixel_access;

            return *this;
        }

        RasterPrint & RasterPrint::operator=(RasterPrint && other) noexcept
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            epsg_index = std::move(epsg_index);
            height = std::move(other.height);
            width = std::move(other.width);
            pixel_size = std::move(other.pixel_size);
            image_matrix = std::move(other.image_matrix);
            pixel_access = std::move(other.pixel_access);
            
            return *this;
        }

        RasterPrint & RasterPrint::operator+=(RasterPrint const& other)
        {
            if(std::abs(pixel_size - other.pixel_size) > std::numeric_limits<double>::epsilon() && reference_point != other.reference_point)
                throw std::logic_error("Case not treated");
            std::transform(
                std::begin(image_matrix),
                std::end(image_matrix),
                std::begin(other.image_matrix),
                std::begin(image_matrix),
                [](const double rhs, const double lhs)
                {
                    return rhs + lhs;
                }
            );
            return *this;
        }
        
        RasterPrint & RasterPrint::operator-=(RasterPrint const& other)
        {
            if( std::abs(pixel_size - other.pixel_size) > std::numeric_limits<double>::epsilon()
                &&
                reference_point != other.reference_point
                &&
                epsg_index != other.epsg_index
              )
                throw std::logic_error("Case not treated");
            std::transform(
                std::begin(image_matrix),
                std::end(image_matrix),
                std::begin(other.image_matrix),
                std::begin(image_matrix),
                [](const double rhs, const double lhs)
                {
                    return rhs - lhs;
                }
            );
            return *this;
        }

        std::ostream & operator<<(std::ostream & os, RasterPrint const& raster_projection)
        {
            std::ios::fmtflags flag_buffer = os.flags();

            os << "Name : " << raster_projection.name << std::endl
               << "Reference (Top left) Point : " << raster_projection.reference_point << std::endl
               << "EPSG code : " << raster_projection.epsg_index << std::endl
               << "Height : " << raster_projection.height << std::endl
               << "Width : " << raster_projection.width << std::endl
               << "Image Matrix : " << std::endl
               << "[[";
            
            os << std::setprecision(6) << std::fixed;

            for(size_t it(0); it < raster_projection.height * raster_projection.width; ++it)
            {
                os << raster_projection.image_matrix.at(it); 
                if((it + 1) % raster_projection.width)
                    os << ", ";
                else
                {
                    os << "]";
                    if(it + 1 != raster_projection.width * raster_projection.height)
                        os << "," << std::endl << " [";
                }
            }            
            os << "]" << std::endl;

            os.flags(flag_buffer);
            return os;
        }

        RasterPrint & operator+(RasterPrint & lhs, RasterPrint const& rhs)
        {
            return lhs += rhs;
        }

        RasterPrint & operator-(RasterPrint & lhs, RasterPrint const& rhs)
        {
            return lhs -= rhs;
        }

        bool operator==(RasterPrint const& lhs, RasterPrint const& rhs)
        {
            bool result(
                std::abs(lhs.pixel_size - rhs.pixel_size) < std::numeric_limits<double>::epsilon()
                &&
                lhs.reference_point == rhs.reference_point
                &&
                lhs.epsg_index == rhs.epsg_index
                &&
                lhs.height == rhs.height
                &&
                lhs.width == rhs.width
            );
            if(result)
            {
                std::vector<bool> comparisons(lhs.height * lhs.width);
                std::transform(
                    std::begin(lhs.image_matrix),
                    std::end(lhs.image_matrix),
                    std::begin(rhs.image_matrix),
                    std::begin(comparisons),
                    [](const double rrhs, const double llhs)
                    {
                        return std::abs(rrhs - llhs) < std::numeric_limits<double>::epsilon();
                    }
                );
                result = std::accumulate(
                    std::begin(comparisons),
                    std::end(comparisons),
                    result,
                    std::logical_and<bool>()
                );
            }
            return result;
        }
        
        bool operator!=(RasterPrint & lhs, RasterPrint const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
