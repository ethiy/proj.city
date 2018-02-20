#include <io/io_gdal.h>

#include <ogr_geometry.h>

#include <algorithm>
#include <iterator>

namespace urban
{
    namespace io
    {
        const std::vector<std::string> FileHandler<GDALDriver>::supported_formats{{"ESRI Shapefile", "GeoJSON", "GML", "KML", "GTiff"}};

        FileHandler<GDALDriver>::FileHandler(GdalFormat const& format, boost::filesystem::path const& _filepath, const std::map<std::string, bool> & _modes)
            : driver_name(supported_formats.at(format)), filepath(_filepath), modes(_modes)
        {
            switch(format)
            {
                case GdalFormat::shapefile :
                    std::cout << "You may face a problem with polygon orientations while reading." << std::endl;
                    break;
                case GdalFormat::geojson :
                    break;
                case GdalFormat::gml :
                    break;
                case GdalFormat::kml :
                    break;
                case GdalFormat::geotiff : 
                    raster = true;
                    break;
                default:
                    throw std::runtime_error("This format is not tested or supported!");
            }
        }

        FileHandler<GDALDriver>::~FileHandler(void) {}

        template<> projection::FootPrint FileHandler<GDALDriver>::read<projection::FootPrint>(void) const
        {
            projection::FootPrint footprint;
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
                        
                        footprint = projection::FootPrint(filepath.stem().string(), file->GetLayer(0));
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

            return footprint;
        }

        void FileHandler<GDALDriver>::write(const projection::FootPrint & footprint, bool labels) const
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

                    footprint.to_ogr(file, labels);
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
                    GDALDriver* driver = GetGDALDriverManager()
                                            ->GetDriverByName(driver_name.c_str());
                    if(driver == NULL)
                    {
                        error_message << "GDAL could not find a driver for: " << driver_name;
                        throw std::runtime_error(error_message.str());
                    }

                    GDALDataset* file =  driver->Create(
                        filepath.string().c_str(),
                        static_cast<int>(raster_image.get_width()),
                        static_cast<int>(raster_image.get_height()),
                        1,
                        GDT_Float64,
                        NULL
                    );

                    raster_image.to_gdal(file);
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
