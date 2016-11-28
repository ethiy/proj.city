#include "io_3ds.h"

template<>
class Reader<Lib3dsFile>
{
public:
    Reader(){}
    Reader(boost::filesystem::path filepath, std::vector<std::string> flags):flags(flags)
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
    std::vector<std::string> flags;
};
