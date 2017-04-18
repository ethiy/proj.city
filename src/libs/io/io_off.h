#pragma once

#include "io.h"

#include "../shadow/mesh.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

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
            FileHandler(void);
            FileHandler(const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes);
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
