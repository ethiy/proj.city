#include <io/io_scene_tree.h>

#include <shadow/bbox.h>

#include <boost/filesystem/operations.hpp>

namespace city
{
    namespace io
    {
        SceneTreeHandler::SceneTreeHandler(boost::filesystem::path const& _filepath)
            : FileHandler(_filepath, std::map<std::string, bool>{{"read", true}})
        {
            auto error = scene_tree.LoadFile(filepath.string().c_str());
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read Scene Tree description!");
        }
        SceneTreeHandler::SceneTreeHandler(boost::filesystem::path const& _filepath, scene::Scene const& scene)
            : FileHandler(_filepath, std::map<std::string, bool>{{"write", true}})
        {
            auto p_root = scene_tree.NewElement("Chantier_Bati3D");
            scene_tree.InsertFirstChild(p_root);

            auto p_bbox = scene_tree.NewElement("Bbox");
            p_root->InsertEndChild(p_bbox);
            set_bbox(p_bbox, scene.bbox());

            auto p_pivot = scene_tree.NewElement("Pivot");
            p_root->InsertEndChild(p_pivot);
            set_pivot(p_pivot, scene.get_pivot());

            set_epsg_index(p_root, scene.get_epsg());

            auto p_citymodel = scene_tree.NewElement("CityModel");
            p_root->InsertEndChild(p_citymodel);
            set_building_ids(p_citymodel, scene.get_identifiers());
            set_terrain_id(p_citymodel, scene.get_terrain().get_name());
        }
        SceneTreeHandler::~SceneTreeHandler(void)
        {}

        shadow::Bbox SceneTreeHandler::bbox(void) const
        {
            double  x_min(0),
                    x_max(0),
                    y_min(0),
                    y_max(0),
                    z_min(0),
                    z_max(0);
            
            auto error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Bbox")->FirstChildElement("Xmin")->QueryDoubleText(&x_min);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read the bounding box");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Bbox")->FirstChildElement("Xmax")->QueryDoubleText(&x_max);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read the bounding box");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Bbox")->FirstChildElement("Ymin")->QueryDoubleText(&y_min);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read the bounding box");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Bbox")->FirstChildElement("Ymax")->QueryDoubleText(&y_max);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read the bounding box");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Bbox")->FirstChildElement("Zmin")->QueryDoubleText(&z_min);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read the bounding box");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Bbox")->FirstChildElement("Zmax")->QueryDoubleText(&z_max);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read the bounding box");
            
            return shadow::Bbox(x_min, x_max, y_min, y_max, z_min, z_max);
        }
        shadow::Point SceneTreeHandler::pivot(void) const
        {
            double  x_offset(0),
                    y_offset(0),
                    z_offset(0);
            
            if(scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot") != nullptr)
            {
                auto error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_x")->QueryDoubleText(&x_offset);
                if(error != tinyxml2::XML_SUCCESS)
                    throw std::runtime_error("Could not read Pivot Point");
                error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_y")->QueryDoubleText(&y_offset);
                if(error != tinyxml2::XML_SUCCESS)
                    throw std::runtime_error("Could not read Pivot Point");
                error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_z")->QueryDoubleText(&z_offset);
                if(error != tinyxml2::XML_SUCCESS)
                    throw std::runtime_error("Could not read Pivot Point");
            }
            else
            {
                shadow::Bbox _bbox = bbox();
                x_offset = (_bbox.xmax() + _bbox.xmin()) / 2.;
                y_offset = (_bbox.ymax() + _bbox.ymin()) / 2.;
                z_offset = (_bbox.zmax() + _bbox.zmin()) / 2.;
            }
            
            return city::shadow::Point(x_offset, y_offset, z_offset);
        }
        unsigned short SceneTreeHandler::epsg_index(void) const
        {
            unsigned int epsg_code = 2154;
            auto error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Code_ESPG_horizontal")->QueryUnsignedText(&epsg_code);
            if(error != tinyxml2::XML_SUCCESS)
                std::cout << "Warning: projection system EPSG code not found! It was set to 2154 - i.e. LAMBERT 93 - by default." << std::endl;
            return static_cast<unsigned short>(epsg_code);
        }
        std::vector<std::string> SceneTreeHandler::building_ids(void) const
        {
            std::vector<std::string> ids;

            tinyxml2::XMLElement const* p_building = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("CityModel")->FirstChildElement("Building");
            while(p_building != nullptr)
            {
                ids.push_back(std::string(p_building->Attribute("Id")));
                p_building = p_building->NextSiblingElement("Building");
            }

            return ids;
        }
        std::string SceneTreeHandler::terrain_id(void) const
        {
            return std::string(
                scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("CityModel")->FirstChildElement("TINRelief")->Attribute("Id")
            );
        }

        SceneTreeHandler& SceneTreeHandler::write(void)
        {
            scene_tree.SaveFile(filepath.string().c_str());
            return *this;
        }

        void SceneTreeHandler::set_bbox(tinyxml2::XMLNode* root, shadow::Bbox const& bbox)
        {
            auto p_xmin = scene_tree.NewElement("Xmin");
            p_xmin->SetText(bbox.xmin());
            root->InsertEndChild(p_xmin);
            auto p_xmax = scene_tree.NewElement("Xmax");
            p_xmax->SetText(bbox.xmax());
            root->InsertEndChild(p_xmax);
            auto p_ymin = scene_tree.NewElement("Ymin");
            p_ymin->SetText(bbox.ymin());
            root->InsertEndChild(p_ymin);
            auto p_ymax = scene_tree.NewElement("Ymax");
            p_ymax->SetText(bbox.ymax());
            root->InsertEndChild(p_ymax);
            auto p_zmin = scene_tree.NewElement("Zmin");
            p_zmin->SetText(bbox.zmin());
            root->InsertEndChild(p_zmin);
            auto p_zmax = scene_tree.NewElement("Zmax");
            p_zmax->SetText(bbox.zmax());
            root->InsertEndChild(p_zmax);
        }
        void SceneTreeHandler::set_pivot(tinyxml2::XMLNode* root, shadow::Point const& pivot)
        {
            auto p_x = scene_tree.NewElement("offset_x");
            p_x->SetText(pivot.x());
            root->InsertEndChild(p_x);
            auto p_y = scene_tree.NewElement("offset_y");
            p_y->SetText(pivot.y());
            root->InsertEndChild(p_y);
            auto p_z = scene_tree.NewElement("offset_z");
            p_z->SetText(pivot.z());
            root->InsertEndChild(p_z);
        }
        void SceneTreeHandler::set_epsg_index(tinyxml2::XMLNode* root, unsigned short const epsg_index)
        {
            auto p_epsg = scene_tree.NewElement("Code_ESPG_horizontal");
            p_epsg->SetText(epsg_index);
            root->InsertEndChild(p_epsg);
        }
        void SceneTreeHandler::set_building_ids(tinyxml2::XMLNode* root, std::vector<std::string> const& building_ids)
        {
            for(auto const& building_id : building_ids)
            {
                auto p_building = scene_tree.NewElement("Building");
                p_building->SetAttribute("Id", building_id.c_str());
                root->InsertEndChild(p_building);
            }
        }
        void SceneTreeHandler::set_terrain_id(tinyxml2::XMLNode* root, std::string const& terrain_id)
        {
            auto p_terrain = scene_tree.NewElement("TINRelief");
            p_terrain->SetAttribute("Id", terrain_id.c_str());
            root->InsertEndChild(p_terrain);
        }
    }
}
