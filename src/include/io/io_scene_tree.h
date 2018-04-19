#pragma once

#include <io/io.h>

#include <scene/scene.h>

#include <tinyxml2.h>

namespace urban
{
    namespace io
    {
        class CityTreeHandler: FileHandler
        {
        public:
            CityTreeHandler(boost::filesystem::path const& _filepath);
            ~CityTreeHandler(void);
            

            shadow::Point pivot(void) const;
            shadow::Bbox bbox(void) const;
            unsigned short epsg_index(void) const;
            std::vector<std::string> building_ids(void) const;
            std::string terrain_id(void) const;
        private:
            tinyxml2::XMLDocument scene_tree;
        };
    }
}
