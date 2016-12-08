#pragma once

#include "io.h"

#include "../UrbanObject/Mesh/mesh.h"

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

            int read(std::vector<urban::Mesh> &);
            int write(std::vector<urban::Mesh>);

        private:
            Lib3dsFile* file;
            boost::filesystem::path filepath;
            int exit_code = EXIT_SUCCESS;
            std::map<std::string, bool> modes;
        };
    }
}
