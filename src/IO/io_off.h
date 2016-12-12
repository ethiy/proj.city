#pragma once

#include "io.h"

#include "../ShadowMesh/shadow_mesh.h"

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
            FileHandler(boost::filesystem::path, std::map<std::string, bool>);
            ~FileHandler(void);

            int read(std::vector<urban::ShadowMesh> &);
            int write(std::vector<urban::ShadowMesh>);

        private:
            std::fstream file;
            boost::filesystem::path filepath;
            int exit_code = EXIT_SUCCESS;
            std::map<std::string, bool> modes;
        };
    }
}
