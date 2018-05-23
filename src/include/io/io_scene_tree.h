#pragma once

#include <io/io.h>

#include <shadow/point.h>
#include <shadow/bbox.h>

#include <scene/scene.h>

#include <tinyxml2.h>

namespace city
{
    namespace io
    {
        class SceneTreeHandler: protected FileHandler
        {
        public:
            SceneTreeHandler(boost::filesystem::path const& _filepath);
            SceneTreeHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene);
            ~SceneTreeHandler(void);

            shadow::Point pivot(void) const;
            shadow::Bbox bbox(void) const;
            unsigned short epsg_index(void) const;
            std::vector<std::string> building_ids(void) const;
            std::string terrain_id(void) const;

            SceneTreeHandler& write(void);
        private:
            tinyxml2::XMLDocument scene_tree;

            void set_bbox(tinyxml2::XMLNode* root, shadow::Point const& pivot, shadow::Bbox const& bbox);
            void set_pivot(tinyxml2::XMLNode* root, shadow::Point const& pivot);
            void set_epsg_index(tinyxml2::XMLNode* root, unsigned short const epsg_index);
            void set_building_ids(tinyxml2::XMLNode* root, std::vector<std::string> const& building_ids);
            void set_terrain_id(tinyxml2::XMLNode* root, std::string const& terrain_id);
        };
    }
}
