#pragma once

#include "io.h"

template<>
class Reader<Lib3dsFile>
{
public:
    Reader();
    Reader(boost::filesystem::path, std::vector<std::string>);
    ~Reader();
    Lib3dsMesh get_meshes();
private:
    Lib3dsFile* input;
    std::vector<std::string> flags;
};
