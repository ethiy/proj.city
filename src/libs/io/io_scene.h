#pragma once

#include "io.h"

#include "../scene/scene.h"

#include <tinyxml2.h>

#include <boost/filesystem/path.hpp>

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
            
            scene::Scene read(void);
        private:
            tinyxml2::XMLDocument scene_tree;
            boost::filesystem::path filepath;

            bool centered = true;

            shadow::Point pivot(void);
            shadow::Bbox bbox(void) const;
            unsigned short epsg_code(void) const;
            std::map<std::size_t, scene::BComposition > structure(void) const;
        };
    }
}
