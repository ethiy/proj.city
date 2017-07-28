#pragma once

#include "io.h"
#include "io_3ds.h"

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
            

            shadow::Point pivot(bool & centered) const;
            shadow::Bbox bbox(void) const;
            unsigned short epsg_index(void) const;
            std::vector<std::string> building_ids(void) const;

            scene::Scene read(FileHandler<Lib3dsFile> const& mesh_file) const;
        private:
            tinyxml2::XMLDocument scene_tree;
            boost::filesystem::path filepath;
        };
    }
}
