#include "io_gdal.h"

#include <ogr_geometry.h>

#include <algorithm>
#include <iterator>

namespace urban
{
    namespace io
    {
        const  std::list<std::string> tested_vector_formats{{"ESRI Shapefile", "GeoJSON", "GML", "KML"}};

        FileHandler<GDALDriver>::FileHandler(void) {}
        FileHandler<GDALDriver>::FileHandler(const std::string & _driver_name, const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes): driver_name(_driver_name), filepath(_filepath), modes(_modes)
        {
            if(driver_name == "ESRI Shapefile")
                std::cout << "You may face a problem with polygon orientations while reading." << std::endl;
            if(std::find(std::begin(tested_vector_formats), std::end(tested_vector_formats), driver_name) == std::end(tested_vector_formats))
                throw std::runtime_error("This format is not tested or supported");
        }

        FileHandler<GDALDriver>::~FileHandler(void) {}

        projection::BrickPrint FileHandler<GDALDriver>::read(void)
        {
            projection::BrickPrint brick_projection;
            std::ostringstream error_message;
            
            if (modes["read"])
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
                GDALAllRegister();
                GDALDriver* esri_driver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str());
                if(esri_driver == NULL)
                {
                    error_message << "GDAL could not find a driver for: " << driver_name;
                    throw std::runtime_error(error_message.str());
                }

                GDALDataset* file = esri_driver->Create(filepath.string().c_str(), 0, 0, 0, GDT_Unknown, NULL);
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
                error_message << std::boolalpha << "The write mode is set to:" << modes["write"] << "! You should set it as follows: \'modes[\"write\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }
        }
    }
}