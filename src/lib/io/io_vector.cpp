#include <io/io_vector.h>

#include <ogr_geometry.h>

#include <algorithm>
#include <iterator>

namespace city
{
    namespace io
    {
        VectorHandler::VectorHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes)
            : FileHandler(_filepath, _modes)
        {}
        VectorHandler::~VectorHandler(void)
        {}

        projection::FootPrint VectorHandler::read(void)
        {
            projection::FootPrint footprint;
            std::ostringstream error_message;
            
            if (modes["read"])
            {
                if (boost::filesystem::is_regular_file(filepath))
                {
                    GDALDataset* file = reinterpret_cast<GDALDataset*>(GDALOpenEx(filepath.string().c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr ));
                    if(file == nullptr)
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
                error_message << std::boolalpha << "The read mode is set to:" << modes["read"] << "! You should set it as follows: \'modes[\"read\"] = true\'";
                boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                throw boost::filesystem::filesystem_error(error_message.str(), ec);
            }

            return footprint;
        }
        void VectorHandler::write(const projection::FootPrint & footprint, bool const labels)
        {
            std::ostringstream error_message;

            if (modes["write"])
            {
                GDALAllRegister();
                GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("ESRI Shapefile");
                if(driver == nullptr)
                    throw std::runtime_error("GDAL could not find a driver for: ESRI Shapefile");

                GDALDataset* file = driver->Create(filepath.string().c_str(), 0, 0, 0, GDT_Unknown, nullptr);
                if(file==nullptr)
                {
                    error_message << "GDAL could not open: \"" << filepath.string() << "\" as an ESRI Shapefile";
                    boost::system::error_code ec(boost::system::errc::io_error, boost::system::system_category());
                    throw boost::filesystem::filesystem_error(error_message.str(), ec);
                }

                footprint.to_ogr(file, labels);
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
