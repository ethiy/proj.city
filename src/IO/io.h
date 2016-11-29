#pragma once

#include <lib3ds/file.h>

#include <boost/filesystem.hpp>

#include <string>
#include <vector>

template<class structure>
class Reader
{
public:
    Reader();
    Reader(boost::filesystem::path, std::vector<std::string>);
    ~Reader();
private:
    structure* input;
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
    std::vector<std::string> flags;
    boost::filesystem::path filepath;
};
