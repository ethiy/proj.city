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

        template<> projection::BrickPrint FileHandler<GDALDriver>::read<projection::BrickPrint>(void) const
        {
            projection::BrickPrint brick_projection;
            std::ostringstream error_message;
            
            if (modes.at("read"))
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
                error_message << std::boolalpha << "The read mode is set to:" << modes.at("read") << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return brick_projection;
        }

        void FileHandler<GDALDriver>::write(const projection::BrickPrint & brick_projection) const
        {
            std::ostringstream error_message;

            if (modes.at("write"))
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
                error_message << std::boolalpha << "The write mode is set to:" << modes.at("write") << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }

        template<> projection::RasterPrint FileHandler<GDALDriver>::read<projection::RasterPrint>(void) const
        {
            projection::RasterPrint raster_projection;
            std::ostringstream error_message;
            
            if (modes.at("read"))
            {
                if(raster)
                {
                    if (boost::filesystem::is_regular_file(filepath))
                    {
                        GDALDataset* file = reinterpret_cast<GDALDataset*>(GDALOpen(filepath.string().c_str(), GA_ReadOnly));
                        if(file == NULL)
                        {
                            error_message << "GDAL could not open: " << filepath.string();
                            throw std::runtime_error(error_message.str());
                        }
                        
                        int epsg_buffer(2154);
                        const char * spatial_reference_system_name = file->GetProjectionRef();
                        if(spatial_reference_system_name)
                        {
                            epsg_buffer = OGRSpatialReference(spatial_reference_system_name).GetEPSGGeogCS();
                        }

                        double geographic_transform[6] = {0,1,0,0,0,1};
                        if( file->GetGeoTransform( geographic_transform ) != CE_None )
                            throw std::runtime_error("GDAL could not retrieve any registered Geometric Transform");
                        
                        GDALRasterBand* raster_band = file->GetRasterBand(1);
                        raster_projection = projection::RasterPrint(filepath.stem().string(), geographic_transform, epsg_buffer, static_cast<size_t>(file->GetRasterYSize()), static_cast<size_t>(file->GetRasterXSize()), raster_band);
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
                    error_message << "The chosen driver : \"" << driver_name << "\" is adapted for raster images";
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }
            }
            else
            {
                error_message << std::boolalpha << "The read mode is set to:" << modes.at("read") << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return raster_projection;
        }

        void FileHandler<GDALDriver>::write(const projection::RasterPrint & raster_image) const
        {
            std::ostringstream error_message;

            if (modes.at("write"))
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

                    size_t height(raster_image.get_height()),
                           width(raster_image.get_width());
                    double adfGeoTransform[6];

                    double* gdal_buffer = const_cast<double*>(raster_image.data());

                    std::array<double, 6> geographic_transform = raster_image.get_geographic_transform();
                    std::copy(std::begin(geographic_transform), std::end(geographic_transform), adfGeoTransform);

                    GDALDataset* file =  driver->Create(filepath.string().c_str(), width, height, 1, GDT_Float64, NULL);

                    file->SetGeoTransform( adfGeoTransform );

                    OGRSpatialReference spatial_reference_system;
                    char* spatial_reference_system_name = NULL;
                    spatial_reference_system.importFromEPSG(2154);
                    spatial_reference_system.exportToWkt(&spatial_reference_system_name);
                    file->SetProjection(spatial_reference_system_name);
                    CPLFree(spatial_reference_system_name);

                    GDALRasterBand* unique_band = file->GetRasterBand(1);
                    CPLErr error = unique_band->RasterIO(GF_Write, 0, 0, width, height, gdal_buffer, width, height, GDT_Float64,0, 0);
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
                error_message << std::boolalpha << "The write mode is set to:" << modes.at("write") << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }
    }
}