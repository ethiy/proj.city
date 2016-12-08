#pragma once

#include "io.h"

#include "../UrbanObject/Mesh/mesh.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <fstream>
#include <map>
#include <string>

template<>
class FileHandler<std::ifstream>
{
public:
    FileHandler(void);
    FileHandler(boost::filesystem::path, std::map<std::string, bool>);
    ~FileHandler(void);

    int read(std::vector<urban::Mesh> &);
    int write(std::vector<urban::Mesh>);

private:
    std::ifstream file;
    boost::filesystem::path filepath;
    int exit_code = EXIT_SUCCESS;
    std::map<std::string, bool> modes;
};
