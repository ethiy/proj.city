#include "io_gdal.h"

#include <ogr_geometry.h>

#include <algorithm>
#include <iterator>

namespace urban
{
    namespace io
    {
        FileHandler<GDALDataset>::FileHandler(void) {}
        FileHandler<GDALDataset>::FileHandler(const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes): filepath(_filepath), modes(_modes) {}
        FileHandler<GDALDataset>::FileHandler(const FileHandler & other): filepath(other.filepath), modes(other.modes) {}
        FileHandler<GDALDataset>::FileHandler(FileHandler && other): filepath(std::move(other.filepath)), modes(std::move(other.modes)) {}
        FileHandler<GDALDataset>::~FileHandler(void)
        {
            std::free(file);
        }

        FileHandler<GDALDataset> & FileHandler<GDALDataset>::operator=(const FileHandler & other) noexcept
        {
            filepath = other.filepath;
            modes = other.modes;
            return *this;
        }

        FileHandler<GDALDataset> & FileHandler<GDALDataset>::operator=(FileHandler && other) noexcept
        {
            filepath = std::move(other.filepath);
            modes = std::move(other.modes);
            return *this;
        }

        projection::BrickPrint & FileHandler<GDALDataset>::read(projection::BrickPrint & brick_projection)
        {
            std::ostringstream error_message;
            
            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    file = reinterpret_cast<GDALDataset*>(GDALOpenEx(filepath.string().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL ));
                    if(file == NULL)
                    {
                        error_message << "GDAL could not open: " << filepath.string();
                        throw std::runtime_error(error_message.str());
                    }
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

        void FileHandler<GDALDataset>::write(const projection::BrickPrint & brick_projection)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                GDALAllRegister();
                const char *esri_driver_name = "ESRI Shapefile";
                GDALDriver* esri_driver = GetGDALDriverManager()->GetDriverByName(esri_driver_name);
                if(esri_driver == NULL)
                {
                    error_message << "GDAL could not find a driver for: " << esri_driver_name;
                    throw std::runtime_error(error_message.str());
                }

                file = esri_driver->Create(filepath.string().c_str(), 0, 0, 0, GDT_Unknown, NULL);
                if(file==NULL)
                {
                    error_message << "GDAL could not open: \"" << filepath.string() << "\" as an " << esri_driver_name;
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }

                OGRLayer* projection_layer = file->CreateLayer("projection", NULL, wkbPolygon, NULL);
                if(projection_layer == NULL)
                    throw std::runtime_error("GDAL could not create a projection layer");
                
                int width(static_cast<int>(brick_projection.size()));
                OGRFieldDefn plane_coefficient_a("Plane coefficient a", OFTReal);
                plane_coefficient_a.SetWidth(width);
                OGRFieldDefn plane_coefficient_b("Plane coefficient b", OFTReal);
                plane_coefficient_a.SetWidth(width);
                OGRFieldDefn plane_coefficient_c("Plane coefficient c", OFTReal);
                plane_coefficient_a.SetWidth(width);
                OGRFieldDefn plane_coefficient_d("Plane coefficient d", OFTReal);
                plane_coefficient_a.SetWidth(width);

                if(
                    (projection_layer->CreateField(&plane_coefficient_a) != OGRERR_NONE ) && (projection_layer->CreateField(&plane_coefficient_b) != OGRERR_NONE )
                    &&
                    (projection_layer->CreateField(&plane_coefficient_c) != OGRERR_NONE ) && (projection_layer->CreateField(&plane_coefficient_d) != OGRERR_NONE )
                  )
                    throw std::runtime_error("GDAL could not create plane coefficient fields");
                
                OGRFeature* polygon_to_write = NULL;
                std::for_each(
                    brick_projection.cbegin(),
                    brick_projection.cend(),
                    [&polygon_to_write, &projection_layer](const projection::FacePrint & facet)
                    {
                        polygon_to_write = NULL;
                        polygon_to_write = OGRFeature::CreateFeature(projection_layer->GetLayerDefn());

                        /* Still has to write Polygon*/

                        Plane_3 plane = facet.get_plane();
                        polygon_to_write->SetField("Plane coefficient a", to_double(plane.a()));
                        polygon_to_write->SetField("Plane coefficient b", to_double(plane.b()));
                        polygon_to_write->SetField("Plane coefficient c", to_double(plane.c()));
                        polygon_to_write->SetField("Plane coefficient d", to_double(plane.d()));
                    }
                );

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