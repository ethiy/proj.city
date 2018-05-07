#include <io/io_raster.h>

#include <algorithm>
#include <iterator>

namespace city
{
    namespace io
    {
        RasterHandler::RasterHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : FileHandler(_filepath, _modes)
        {}
        RasterHandler::~RasterHandler(void)
        {}

        projection::RasterPrint RasterHandler::read(void)
        {
            projection::RasterPrint raster_projection;
            std::ostringstream error_message;
            
            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    GDALDataset* file = reinterpret_cast<GDALDataset*>(GDALOpen(filepath.string().c_str(), GA_ReadOnly));
                    if(file == nullptr)
                    {
                        error_message << "GDAL could not open: " << filepath.string();
                        throw std::runtime_error(error_message.str());
                    }
                    
                    raster_projection = projection::RasterPrint(filepath.stem().string(), file);
                    GDALClose(dynamic_cast<GDALDatasetH>(file));
                }
                else
                {
                    error_message << "This file \"" << filepath.string() << "\" cannot be found! You should check the file path";
                    boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return raster_projection;
        }
        
        void RasterHandler::write(const projection::RasterPrint & raster_image)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                GDALAllRegister();
                GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("GTiff");
                if(driver == nullptr)
                {
                    error_message << "GDAL could not find a driver for: GeoTiff";
                    throw std::runtime_error(error_message.str());
                }

                GDALDataset* file = driver->Create(
                    filepath.string().c_str(),
                    static_cast<int>(raster_image.get_width()),
                    static_cast<int>(raster_image.get_height()),
                    1,
                    GDT_Float64,
                    nullptr
                );

                raster_image.to_gdal(file);
                GDALClose(dynamic_cast<GDALDatasetH>(file));
            }
            else
            {
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }
    }
}
