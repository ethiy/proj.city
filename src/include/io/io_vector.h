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
        class VectorHandle: FileHandler
        {
        public:
            VectorHandle(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~VectorHandle(void);

            projection::RasterPrint read(void) const;

            void write(projection::RasterPrint const& raster_image) const;
        };
    }
}
