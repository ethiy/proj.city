#include "io_3ds.h"

template<> class Reader<Lib3dsFile>::Reader(){}
template<> class Reader<Lib3dsFile>::Reader(boost::filesystem::path filepath, std::vector<std::string> flags): flags(flags)
{
    input = lib3ds_file_load(filepath.string().c_string());
}
template<> class Reader<Lib3dsFile>::~Reader(){}

Lib3dsMesh template<> class Reader<Lib3dsFile>::get_meshes()
{
    return input->meshes;
}
