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
            FileHandler(std::string const& driver_name, boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~FileHandler(void);

            template<class P>
            P read(void) const;

            void write(projection::BrickPrint const& brick_projection) const;
            void write(projection::RasterPrint const& raster_image) const;
        private:
            std::string driver_name;
            bool raster = false;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
        
        template<>
        projection::BrickPrint FileHandler<GDALDriver>::read(void) const;
        template<>
        projection::RasterPrint FileHandler<GDALDriver>::read(void) const;
    }
}
