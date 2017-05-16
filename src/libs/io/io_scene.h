#pragma once

#include "io.h"

#include "../shadow/Point/point.h"

#include <tinyxml2.h>

#include <boost/filesystem/path.hpp>

#include <string>
#include <map>
#include <set>

namespace urban
{
    namespace io
    {
        template<>
        class FileHandler<tinyxml2::XMLDocument>
        {
        public:
            FileHandler(boost::filesystem::path const& _filepath);
            ~FileHandler(void);
            
            shadow::Point pivot(void) const;
            unsigned short epsg_code(void) const;
            std::map<std::size_t, std::pair< std::set<std::string>, std::set<std::string> > > buildings(void) const;
        private:
            tinyxml2::XMLDocument scene_tree;
            boost::filesystem::path filepath;
        };
    }
}
