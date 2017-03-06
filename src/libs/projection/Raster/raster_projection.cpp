#include "raster_projection.h"

#include <iterator>
#include <algorithm>

#include <cmath>

namespace urban
{
    namespace projection
    {
        RasterPrint::RasterPrint(void) {}
        RasterPrint::RasterPrint(const RasterPrint & other): name(other.name), reference_point(other.reference_point), length(other.length), width(other.width), pixel_size(other.pixel_size), pixels(other.pixels) {}
        RasterPrint::RasterPrint(RasterPrint && other): name(std::move(other.name)), reference_point(std::move(other.reference_point)), length(std::move(other.length)), width(std::move(other.width)), pixel_size(std::move(other.pixel_size)), pixels(std::move(other.pixels)) {}
        RasterPrint::~RasterPrint(void) {}

        void RasterPrint::swap(RasterPrint & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(length, other.length);
            swap(width, other.width);
            swap(pixel_size, other.pixel_size);
            swap(pixels, other.pixels);
        }

        RasterPrint & RasterPrint::operator=(const RasterPrint & other) noexcept
        {
            name = other.name;
            reference_point = other.reference_point;
            length = other.length;
            width = other.width;
            pixel_size = other.pixel_size;
            pixels = other.pixels;

            return *this;
        }

        RasterPrint & RasterPrint::operator=(RasterPrint && other) noexcept
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            length = std::move(other.length);
            width = std::move(other.width);
            pixel_size = std::move(other.pixel_size);
            pixels = std::move(other.pixels);
            
            return *this;
        }

        RasterPrint & RasterPrint::rescale(double _pixel_size)
        {
            if(_pixel_size != pixel_size)
            {
                length = std::ceil(length * pixel_size / _pixel_size);
                width = std::ceil(width * pixel_size / _pixel_size);
            }
            pixel_size = _pixel_size;
            return *this;
        }

        RasterPrint & RasterPrint::operator+=(const RasterPrint & other)
        {
            name += "+" + other.name;
            if(pixel_size !=  other.pixel_size)
                throw std::logic_error("Not yet implemented");
            size = std::min(pixel_size, other.pixel_size);
            rescale(size);

            other.rescale(size);
            throw std::logic_error("Not yet implemented");
            return *this;
        }

        RasterPrint & RasterPrint::operator-=(const RasterPrint & other)
        {
            name += "+" + other.name;
            pixel_size = std::min(pixel_size, other.pixel_size);
            throw std::logic_error("Not yet implemented");
            return *this;
        }


    }

    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
