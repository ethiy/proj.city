#pragma once

#include <io/io.h>

#include <projection/raster_projection.h>

#include <ogrsf_frmts.h>

#include <map>
#include <vector>
#include <string>

namespace urban
{
    namespace io
    {
        class RasterHandle: FileHandler
        {
        public:
            RasterHandle(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~RasterHandle(void);

            projection::RasterPrint read(void) const;

            void write(projection::RasterPrint const& raster_image) const;
        };
    }
}
