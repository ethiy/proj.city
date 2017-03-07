#pragma once

#include "io.h"
#include "../projection/Brick/brick_projection.h"
#include "../projection/Raster/raster_projection.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <ogrsf_frmts.h>

#include <map>
#include <list>
#include <vector>
#include <string>

namespace urban
{
    namespace io
    {
        extern const std::list<std::string> tested_vector_formats;
        extern const std::list<std::string> tested_raster_formats;

        template<>
        class FileHandler<GDALDriver>
        {
        public:
            FileHandler(void);
            FileHandler(const std::string & driver_name, const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes);
            ~FileHandler(void);

            template<class return_class> return_class read(void) const;

            void write(const projection::BrickPrint & brick_projection) const;
            void write(const projection::RasterPrint & raster_image) const;
        private:
            std::string driver_name;
            bool raster;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
