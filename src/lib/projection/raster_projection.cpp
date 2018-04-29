#include <projection/raster_projection.h>

#include <shadow/vector.h>

#include <cpl_string.h>

#include <iterator>
#include <algorithm>
#include <iomanip>

#include <cmath>

namespace city
{
    namespace projection
    {
        RasterPrint::RasterPrint(void)
        {}
        RasterPrint::RasterPrint(FootPrint const& footprint, double const _pixel_size)
            : name(footprint.get_name()),
              reference_point(footprint.get_reference_point() + shadow::Vector(footprint.bbox().xmin(), footprint.bbox().ymax(), 0)),
              epsg_index(footprint.get_epsg()),
              height(static_cast<std::size_t>(std::ceil((footprint.bbox().ymax() - footprint.bbox().ymin()) / _pixel_size))),
              width(static_cast<std::size_t>(std::ceil((footprint.bbox().xmax() - footprint.bbox().xmin()) / _pixel_size))),
              pixel_size(_pixel_size),
              image_matrix(height * width, 0.),
              pixel_hits(height * width, 0)
        {
            image_matrix = std::accumulate(
                std::begin(footprint),
                std::end(footprint),
                image_matrix,
                [this, &footprint](std::vector<double> & image, projection::FacePrint const& face_projection)
                {
                    return face_projection.rasterize(image, pixel_hits, shadow::Point(footprint.bbox().xmin(), footprint.bbox().ymax(), 0), height, width, pixel_size);
                }
            );
            vertical_offset();
        }
        RasterPrint::RasterPrint(std::string const& filename, GDALDataset* raster_file)

            : name(filename),
              height(static_cast<std::size_t>(raster_file->GetRasterYSize())),
              width(static_cast<std::size_t>(raster_file->GetRasterXSize())),
              pixel_hits(height * width, 1),
              offset(true)
        {
            int epsg_buffer(2154);

            const char * spatial_reference_system_name = raster_file->GetProjectionRef();
            if(spatial_reference_system_name)
            {
                epsg_buffer = OGRSpatialReference(spatial_reference_system_name).GetEPSGGeogCS();
            }
            epsg_index = epsg_buffer > 0 ? static_cast<unsigned short>(epsg_buffer) : 2154;

            double geographic_transform[6] = {0, .06, 0, 0, 0, .06};
            if( raster_file->GetGeoTransform( geographic_transform ) != CE_None )
                throw std::runtime_error("GDAL could not retrieve any registered Geometric Transform");
            if(std::abs(geographic_transform[1] + geographic_transform[5]) > std::numeric_limits<double>::epsilon())
                throw std::logic_error("this case is not treated yet!");
            
            reference_point = shadow::Point(geographic_transform[0], geographic_transform[3], 0);
            pixel_size = geographic_transform[1];
            
            double* buffer = reinterpret_cast<double*>(calloc(sizeof(double), width * height));
            GDALRasterBand* raster_band = raster_file->GetRasterBand(1);
            CPLErr error = raster_band->RasterIO(GF_Read, 0, 0, static_cast<int>(width), static_cast<int>(height), buffer, static_cast<int>(width), static_cast<int>(height), GDT_Float64, 0, 0);
            if(error != CE_None)
                throw std::runtime_error("GDAL could not read raster band");

            image_matrix = std::vector<double>(buffer, buffer + width * height);
        }
        RasterPrint::RasterPrint(RasterPrint const& other)
            : name(other.name),
              reference_point(other.reference_point),
              epsg_index(other.epsg_index),
              height(other.height),
              width(other.width),
              pixel_size(other.pixel_size),
              image_matrix(other.image_matrix),
              pixel_hits(other.pixel_hits),
              offset(other.offset)
        {}
        RasterPrint::RasterPrint(RasterPrint && other)
            : name(std::move(other.name)),
              reference_point(std::move(other.reference_point)),
              epsg_index(std::move(other.epsg_index)),
              height(std::move(other.height)),
              width(std::move(other.width)),
              pixel_size(std::move(other.pixel_size)),
              image_matrix(std::move(other.image_matrix)),
              pixel_hits(std::move(other.pixel_hits)),
              offset(std::move(other.offset))
        {}
        RasterPrint::~RasterPrint(void)
        {}


        std::string const& RasterPrint::get_name(void) const noexcept
        {
            return name;
        }

        std::size_t RasterPrint::get_height(void) const noexcept
        {
            return height;
        }

        std::size_t RasterPrint::get_width(void) const noexcept
        {
            return width;
        }

        std::size_t RasterPrint::get_index(std::size_t const& i, std::size_t const& j) const noexcept
        {
            return i * width + j;
        }

        shadow::Point const& RasterPrint::get_reference_point() const noexcept
        {
            return reference_point;
        }
        unsigned short const& RasterPrint::get_epsg() const noexcept
        {
            return epsg_index;
        }
        double const& RasterPrint::get_pixel_size() const noexcept
        {
            return pixel_size;
        }

        double* RasterPrint::data(void) noexcept
        {
            return image_matrix.data();
        }
        double const* RasterPrint::data(void) const noexcept
        {
            return image_matrix.data();
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
            swap(pixel_hits, other.pixel_hits);
            swap(offset, other.offset);
        }

        double & RasterPrint::at(std::size_t const& i, std::size_t const& j)
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return image_matrix.at(i * width + j);
        }
        double const& RasterPrint::at(std::size_t const& i, std::size_t const& j) const
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return image_matrix.at(i * width + j);
        }
        short & RasterPrint::hit(std::size_t const& i, std::size_t const& j)
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return pixel_hits.at(i * width + j);
        }
        const short & RasterPrint::hit(std::size_t const& i, std::size_t const& j) const
        {
            if(i>height && j>width)
                throw std::out_of_range("You iz out of rangez!!");
            return pixel_hits.at(i * width + j);
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

        RasterPrint & RasterPrint::operator =(RasterPrint const& other) noexcept
        {
            name = other.name;
            reference_point = other.reference_point;
            epsg_index = other.epsg_index;
            height = other.height;
            width = other.width;
            pixel_size = other.pixel_size;
            image_matrix = other.image_matrix;
            pixel_hits = other.pixel_hits;
            offset = other.offset;

            return *this;
        }

        RasterPrint & RasterPrint::operator =(RasterPrint && other) noexcept
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            epsg_index = std::move(other.epsg_index);
            height = std::move(other.height);
            width = std::move(other.width);
            pixel_size = std::move(other.pixel_size);
            image_matrix = std::move(other.image_matrix);
            pixel_hits = std::move(other.pixel_hits);
            offset = std::move(other.offset);
            
            return *this;
        }

        void RasterPrint::vertical_offset(void)
        {
            if(!offset)
            {
                for(std::size_t index(0); index != height * width; ++index)
                    image_matrix.at(index) += (pixel_hits.at(index) != 0) * reference_point.z();

                offset = true;
            }
        }

        void RasterPrint::set_geotransform(GDALDataset* file) const
        {
            double adfGeoTransform[6] = {reference_point.x(), pixel_size, 0, reference_point.y(), 0, - pixel_size};
            file->SetGeoTransform(adfGeoTransform);
        }

        void RasterPrint::set_projection(GDALDataset* file) const
        {
            OGRSpatialReference spatial_reference_system;
            char* spatial_reference_system_name = nullptr;
            spatial_reference_system.importFromEPSG(epsg_index);
            spatial_reference_system.exportToWkt(&spatial_reference_system_name);
            file->SetProjection(spatial_reference_system_name);
            CPLFree(spatial_reference_system_name);
        }
        
        void RasterPrint::save_image(GDALDataset* file) const
        {
            GDALRasterBand* unique_band = file->GetRasterBand(1);
            CPLErr error = unique_band->RasterIO(
                GF_Write,
                0,
                0,
                static_cast<int>(width),
                static_cast<int>(height),
                const_cast<double*>(image_matrix.data()),
                static_cast<int>(width),
                static_cast<int>(height),
                GDT_Float64,
                0,
                0
            );
            if(error != CE_None)
                throw std::runtime_error("GDAL could not save raster band");
        }

        void RasterPrint::to_gdal(GDALDataset* file) const
        {
            set_geotransform(file);
            set_projection(file);
            save_image(file);
        }

        std::ostream & operator <<(std::ostream & os, RasterPrint const& raster_projection)
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

            for(std::size_t it(0); it < raster_projection.height * raster_projection.width; ++it)
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

        bool RasterPrint::equal_metadata(RasterPrint const& other) const
        {
            return  std::abs(pixel_size - other.pixel_size) < std::numeric_limits<double>::epsilon()
                    &&
                    reference_point == other.reference_point
                    &&
                    epsg_index == other.epsg_index
                    &&
                    height == other.height
                    &&
                    width == other.width;
        }

        bool operator ==(RasterPrint const& lhs, RasterPrint const& rhs)
        {
            bool result = lhs.equal_metadata(rhs);
            if(result)
            {
                std::inner_product(
                    std::begin(lhs.image_matrix),
                    std::end(lhs.image_matrix),
                    std::begin(rhs.image_matrix),
                    result,
                    std::logical_and<bool>(),
                    [](const double _rhs, const double _lhs)
                    {
                        return std::abs(_rhs - _lhs) < std::numeric_limits<double>::epsilon();
                    }
                );
            }
            return result;
        }
        
        bool operator !=(RasterPrint & lhs, RasterPrint const& rhs)
        {
            return !(lhs == rhs);
        }
    }

    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs)
    {
        lhs.swap(rhs);
    }
}
