#pragma once

#include <io/io.h>

#include <shadow/mesh.h>
#include <io/Off_stream/off_stream.h>

#include <boost/filesystem/path.hpp>

#include <fstream>
#include <map>
#include <string>

namespace urban
{
    namespace io
    {
        template<>
        class FileHandler<std::fstream>
        {
        public:
            FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~FileHandler(void);
            
            shadow::Mesh read(void);
            void write(shadow::Mesh const& mesh);

        private:
            std::fstream file;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
