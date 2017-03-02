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
        class FileHandler<GDALDriver>
        {
        public:
            FileHandler(void);
            FileHandler(const std::string & driver_name, const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes);
            ~FileHandler(void);

            projection::BrickPrint read(void);
            void write(const projection::BrickPrint & brick_projection);

        private:
            std::string driver_name;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
