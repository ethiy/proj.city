#pragma once

#include <io/io.h>

#include <shadow/mesh.h>


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
