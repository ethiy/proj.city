#include "io_gdal.h"

#include <ogr_geometry.h>
#include "cpl_string.h"

#include <algorithm>
#include <iterator>

namespace urban
{
    namespace io
    {
        const std::list<std::string> tested_vector_formats{{"ESRI Shapefile", "GeoJSON", "GML", "KML"}};
        const std::list<std::string> tested_raster_formats{{"GTiff"}};

        FileHandler<GDALDriver>::FileHandler(void) {}
        FileHandler<GDALDriver>::FileHandler(const std::string & _driver_name, const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes): driver_name(_driver_name), filepath(_filepath), modes(_modes)
        {
            if(driver_name == "ESRI Shapefile")
                std::cout << "You may face a problem with polygon orientations while reading." << std::endl;
            
            std::list<std::string>::const_iterator vector_driver_it = std::find(std::begin(tested_vector_formats), std::end(tested_vector_formats), driver_name);
            std::list<std::string>::const_iterator raster_driver_it = std::find(std::begin(tested_raster_formats), std::end(tested_raster_formats), driver_name);

            if(vector_driver_it == std::end(tested_vector_formats)
                &&
               raster_driver_it == std::end(tested_raster_formats)
              )
                throw std::runtime_error("This format is not tested or supported");
            else
            {
                if(vector_driver_it != std::end(tested_vector_formats) && raster_driver_it == std::end(tested_raster_formats))
                    raster = false;
                else
                {
                    if(raster_driver_it != std::end(tested_raster_formats))
                        raster = true;
                    else
                        throw std::logic_error("The driver cannot be for raster and vector in the same time");
                }
            }
        }

        FileHandler<GDALDriver>::~FileHandler(void) {}

        template<> projection::BrickPrint FileHandler<GDALDriver>::read<urban::projection::BrickPrint>(void)
        {
            projection::BrickPrint brick_projection;
            std::ostringstream error_message;
            
            if (modes["read"])
            {
                if(!raster)
                {
                    if (boost::filesystem::is_regular_file(filepath))
                    {
                        GDALDataset* file = reinterpret_cast<GDALDataset*>(GDALOpenEx(filepath.string().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL ));
                        if(file == NULL)
                        {
                            error_message << "GDAL could not open: " << filepath.string();
                            throw std::runtime_error(error_message.str());
                        }
                        
                        brick_projection = projection::BrickPrint(filepath.stem().string(), file->GetLayer(0));
                        GDALClose(file);
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
                    error_message << "The chosen driver : \"" << driver_name << "\" is adapted for vectorial images";
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return brick_projection;
        }

        void FileHandler<GDALDriver>::write(const projection::BrickPrint & brick_projection)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                if(!raster)
                {
                    GDALAllRegister();
                    GDALDriver* driver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());
                    if(driver == NULL)
                    {
                        error_message << "GDAL could not find a driver for: " << driver_name;
                        throw std::runtime_error(error_message.str());
                    }

                    GDALDataset* file = driver->Create(filepath.string().c_str(), 0, 0, 0, GDT_Unknown, NULL);
                    if(file==NULL)
                    {
                        error_message << "GDAL could not open: \"" << filepath.string() << "\" as an " << driver_name;
                        boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                        throw boost::filesystem::filesystem_error(error_message.str(), ec);
                    }

                    brick_projection.to_ogr(file);
                    GDALClose(file);
                }
                else
                {
                    error_message << "The chosen driver : \"" << driver_name << "\" is adapted for vectorial images";
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            else
            {
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }

        template<> std::vector<double> FileHandler<GDALDriver>::read< std::vector<double> >(void)
        {
            return std::vector<double>();
        }

        void FileHandler<GDALDriver>::write(const std::vector<uint16_t> & raster_image, const std::pair<size_t, size_t> & image_sizes)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                if(raster)
                {
                    GDALAllRegister();
                    GDALDriver* driver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());
                    if(driver == NULL)
                    {
                        error_message << "GDAL could not find a driver for: " << driver_name;
                        throw std::runtime_error(error_message.str());
                    }

                    char **options = NULL;
                    GDALDataset* file =  driver->Create(filepath.string().c_str(), image_sizes.second, image_sizes.first, 1, GDT_UInt16, options);

                    double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
                    file->SetGeoTransform( adfGeoTransform );

                    OGRSpatialReference spatial_reference_system;
                    char* spatial_reference_system_name = NULL;
                    spatial_reference_system.importFromEPSG(2154);
                    spatial_reference_system.exportToWkt(&spatial_reference_system_name);
                    file->SetProjection(spatial_reference_system_name);
                    CPLFree(spatial_reference_system_name);

                    GDALRasterBand* unique_band = file->GetRasterBand(1);
                    uint16_t gdal_image[image_sizes.first * image_sizes.second];
                    std::copy(std::begin(raster_image), std::end(raster_image), gdal_image);
                    CPLErr error = unique_band->RasterIO(GF_Write, 0, 0, image_sizes.second, image_sizes.first, gdal_image, image_sizes.second, image_sizes.first, GDT_UInt16,0, 0);
                    if(error != CE_None)
                        throw std::runtime_error("GDAL could not save raster band");
                    GDALClose(dynamic_cast<GDALDatasetH>(file));
                }
                else
                {
                    error_message << "The chosen driver : \"" << driver_name << "\" is adapted for raster images";
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
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