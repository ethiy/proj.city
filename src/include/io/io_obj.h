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
        template<>
        class FileHandler<Obj_stream>
        {
        public:
            FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~FileHandler(void);
            
            std::vector<shadow::Mesh> read(void);
            void write(std::vector<shadow::Mesh> const& mesh);

        private:
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
