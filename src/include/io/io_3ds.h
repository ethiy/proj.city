#pragma once

#include <io/io.h>

#include <shadow/mesh.h>

#include <lib3ds/file.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <map>
#include <vector>
#include <set>
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

            std::vector<shadow::Mesh> read(void) const;
            shadow::Mesh read(std::string const& node_name, std::set<char> const& facet_types) const;
            std::vector<shadow::Mesh> read(std::size_t const level, std::set<char> const& facet_types) const;

            std::vector<shadow::Mesh> read_tmps(std::string const& node_name, std::set<char> const& facet_types) const;
            std::vector<std::vector<shadow::Mesh> > read_tmps(std::size_t const level, std::set<char> const& facet_types) const;

            std::map<char, std::deque<shadow::Mesh> > get_meshes(std::string const& node_name, std::set<char> const& facet_types) const;
            std::map<char, shadow::Mesh> get_mesh_by_type(std::string const& node_name, std::set<char> const& facet_types) const;
            std::vector<std::string> get_nodes(std::size_t const level) const;

            void write(std::vector<shadow::Mesh> meshes);
        private:
            Lib3dsFile* file = NULL;
            boost::filesystem::path filepath;
            std::map<std::string, bool> modes;

            void node_meshes(Lib3dsNode * node, std::map<char, std::deque<shadow::Mesh> > & meshes, std::set<char> const& facet_types) const;
        };
    }
}
