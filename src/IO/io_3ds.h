#pragma once

#include "io.h"

template<> class Reader<Lib3dsFile>;
template<> class Writer<Lib3dsFile>;

template<>
class Reader<Lib3dsFile>
{
public:
    Reader(){}
    Reader(boost::filesystem::path filepath, std::vector<std::string> flags):filepath(filepath), flags(flags)
    {
        input = lib3ds_file_load( filepath.string().c_str());
    }
    ~Reader(){}
    Lib3dsMesh* get_meshes()
    {
        return input->meshes;
    }

private:
    Lib3dsFile* input;
    boost::filesystem::path filepath;
    std::vector<std::string> flags;
};

template<>
class Writer<Lib3dsFile>
{
public:
    Writer(){}
    Writer(boost::filesystem::path filepath, std::vector<std::string> flags, Lib3dsMesh* meshes):filepath(filepath), flags(flags)
    {
        lib3ds_file_insert_mesh(output, meshes);
    }
    ~Writer(){}
    void save()
    {
        lib3ds_file_save(output, filepath.string().c_str());
    }

private:
    Lib3dsFile* output;
    boost::filesystem::path filepath;
    std::vector<std::string> flags;
};
