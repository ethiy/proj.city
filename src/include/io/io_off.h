#pragma once

#include <io/io.h>

#include <shadow/mesh.h>

namespace urban
{
    namespace io
    {
        class OFFHandler: protected FileHandler
        {
        public:
            OFFHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~OFFHandler(void);
            
            shadow::Mesh read(void) const;
            void write(shadow::Mesh const& mesh) const;
        };
    }
}
