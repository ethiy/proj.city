#include "io_gdal.h"

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
            file = NULL;
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
            return brick_projection;
        }

        void FileHandler<GDALDataset>::write(const projection::BrickPrint & brick_projection)
        {
            return ;
        }
    }
}