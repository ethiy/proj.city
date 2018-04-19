#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <io/Obj_stream/obj_stream.h>

#include <boost/filesystem/path.hpp>

#include <vector>

namespace urban
{
    namespace io
    {
        class WaveObjHandler: FileHandler
        {
        public:
            WaveObjHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~WaveObjHandler(void);
            
            std::vector<shadow::Mesh> read(void);
            void write(std::vector<shadow::Mesh> const& mesh);
        };
    }
}
