#pragma once

#include <io/io.h>

#include <shadow/mesh.h>

#include <lib3ds/file.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <map>
#include <vector>
#include <string>

namespace urban
{
    namespace io
    {
        template<>
        class FileHandler<Lib3dsFile>
        {
        public:
            FileHandler(boost::filesystem::path const& _filepath, std::map<std::string, bool> const& _modes);
            ~FileHandler(void);

            std::vector<shadow::Mesh> read(std::string const& node_name) const;
            shadow::Mesh read_and_stitch(std::string const& node_name) const;
            std::vector<shadow::Mesh> read_roofs(std::string const& node_name) const;
            std::vector<shadow::Mesh> read(void) const;
            std::vector<shadow::Mesh> read_level(std::size_t const level) const; 
            std::vector<std::string> get_nodes(std::size_t const level) const;

            void write(std::vector<shadow::Mesh> meshes);
        private:
            Lib3dsFile* file = NULL;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;

            void node_meshes(Lib3dsNode * node, std::vector<shadow::Mesh> & meshes) const;
            void roof_nodes(Lib3dsNode * node, std::vector<shadow::Mesh> & meshes) const;
        };
    }
}
