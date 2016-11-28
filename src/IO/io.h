#pragma once

#include <lib3ds/file.h>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>
#include <istream>

template<class structure>
class Reader
{
public:
    Reader();
    Reader(boost::filesystem::path, std::vector<std::string>);
    ~Reader();
private:
    structure* input;
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
};

template<> class Reader<Lib3dsFile>;
template<> class Writer<Lib3dsFile>;
