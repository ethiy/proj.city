#pragma once

#include "io.h"

#include "../ShadowMesh/shadow_mesh.h"

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
            FileHandler(boost::filesystem::path, std::map<std::string, bool>);
            ~FileHandler(void);

            std::vector<ShadowMesh> read(void);
            void write(std::vector<ShadowMesh>);

        private:
            Lib3dsFile* file = NULL;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;
        };
    }
}
