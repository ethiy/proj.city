#pragma once

#include <io/io.h>

#include <projection/scene_projection.h>

#include <ogrsf_frmts.h>

#include <map>
#include <vector>
#include <string>

namespace urban
{
    namespace io
    {
        class VectorHandler: protected FileHandler
        {
        public:
            VectorHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~VectorHandler(void);

            projection::FootPrint read(void);

            void write(projection::FootPrint const& vectorimage, bool const label = true);
        };
    }
}
