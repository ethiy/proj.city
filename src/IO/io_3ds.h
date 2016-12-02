#pragma once

#include "io.h"

#include <lib3ds/file.h>

#include <boost/filesystem/operations.hpp>

#include <iostream>
#include <vector>

template<> class Reader<Lib3dsFile>;
template<> class Writer<Lib3dsFile>;

template<>
class Reader<Lib3dsFile>
{
public:
    Reader(){}

    Reader(boost::filesystem::path filepath, std::vector<std::string> flags):filepath(filepath), flags(flags)
    {
        try
        {
            if(boost::filesystem::is_regular_file(filepath))
            {
                input = lib3ds_file_load( filepath.string().c_str());
                open = true;
            }
            else
            {
                boost::system::error_code ec(boost::system::errc::no_such_file_or_directory, boost::system::system_category());
                throw boost::filesystem::filesystem_error(ec.message(), ec);
                exit_code = boost::system::errc::no_such_file_or_directory;
            }
        }
        catch(const boost::filesystem::filesystem_error& error)
        {
            std::cerr << "Reader( " << filepath << ", ...) failed with: " << error.code().message() << std::endl;
        }
    }

    ~Reader(){}

    int get_exit_code(void)
    {
        return exit_code;
    }

    void get_facets(std::vector<urban::_Mesh> & _meshes)
    {
        if(open)
        {
            Lib3dsMesh* meshes = input->meshes;
            while(meshes)
            {
                _meshes.push_back(urban::_Mesh(*meshes));
                meshes = meshes->next;
            }
        }
    }

private:
    Lib3dsFile* input;
    bool open = false;
    int exit_code = EXIT_SUCCESS;
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
    bool open;
    int exit_code;
    boost::filesystem::path filepath;
    std::vector<std::string> flags;
};
