#pragma once

#include "../UrbanObject/Mesh/mesh.h"

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>


template<class structure>
class FileHandler;

template<class structure>
class Reader
{
public:
    Reader();
    Reader(boost::filesystem::path, std::vector<std::string>);
    ~Reader();

    int get_exit_code(void);
    void get_facets(std::vector<urban::Mesh> &);
private:
    structure* input;
    bool open;
    int exit_code;
    boost::filesystem::path filepath;
    std::vector<std::string> flags;
};

template<class structure>
class Writer
{
public:
    Writer();
    Writer(boost::filesystem::path, std::vector<std::string>);
    ~Writer();
private:
    structure* output;
    bool open;
    int exit_code;
    std::vector<std::string> flags;
    boost::filesystem::path filepath;
};
