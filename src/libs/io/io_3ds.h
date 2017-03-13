#pragma once

#include "io.h"

#include "../shadow/mesh.h"

#include <lib3ds/file.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <map>
#include <string>

namespace urban
{
    namespace io
    {
        template<>
        class FileHandler<Lib3dsFile>
        {
        public:
            FileHandler(void);
            FileHandler(const boost::filesystem::path & _filepath, const std::map<std::string, bool> & _modes);
            ~FileHandler(void);

            std::vector<shadow::Mesh> read(void);
            void write(std::vector<shadow::Mesh> meshes);

        private:
            Lib3dsFile* file = NULL;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
