#pragma once

#include <io/io.h>

#include <shadow/point.h>
#include <shadow/bbox.h>

#include <tinyxml2.h>

namespace urban
{
    namespace io
    {
        class SceneTreeHandler: protected FileHandler
        {
        public:
            SceneTreeHandler(boost::filesystem::path const& _filepath);
            ~SceneTreeHandler(void);

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
