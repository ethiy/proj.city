#pragma once

#include "io.h"
#include "../Projection/Brick/brick_projection.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <ogrsf_frmts.h>

#include <map>
#include <string>

namespace urban
{
    namespace io
    {
        template<>
        class FileHandler<GDALDataset>
        {
        public:
            FileHandler(void);
            FileHandler(const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes);
            FileHandler(const FileHandler & other);
            FileHandler(FileHandler && other);
            ~FileHandler(void);

            FileHandler<GDALDataset> & operator=(const FileHandler & other) noexcept;
            FileHandler<GDALDataset> & operator=(FileHandler && other) noexcept;

            projection::BrickPrint & read(projection::BrickPrint & brick_projection);
            void write(const projection::BrickPrint & brick_projection);

        private:
            GDALDataset* file = NULL;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
