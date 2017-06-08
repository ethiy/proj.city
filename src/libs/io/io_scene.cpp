#include "io_scene.h"

#include "../shadow/Bbox/bbox.h"

#include <boost/filesystem/operations.hpp>

namespace urban
{
    namespace io
    {
        FileHandler<tinyxml2::XMLDocument>::FileHandler(boost::filesystem::path const& _filepath)
            : filepath(_filepath)
        {
            auto error = scene_tree.LoadFile(filepath.string().c_str());
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read Scene Tree description!");
        }
        FileHandler<tinyxml2::XMLDocument>::~FileHandler(void)
        {}

        scene::Scene FileHandler<tinyxml2::XMLDocument>::read(void)
        {
            return scene::Scene(pivot(), centered, epsg_code(), structure());
        }

        shadow::Bbox FileHandler<tinyxml2::XMLDocument>::bbox(void) const
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

        shadow::Point FileHandler<tinyxml2::XMLDocument>::pivot(void)
        {
            double  x_offset(0),
                    y_offset(0),
                    z_offset(0);
            
            if(scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot") != NULL)
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
                centered = false;
                shadow::Bbox _bbox = bbox();
                x_offset = (_bbox.xmax() + _bbox.xmin()) / 2.;
                y_offset = (_bbox.ymax() + _bbox.ymin()) / 2.;
            }
            
            return urban::shadow::Point(x_offset, y_offset, z_offset);
        }

        unsigned short FileHandler<tinyxml2::XMLDocument>::epsg_code(void) const
        {
            unsigned int epsg_code = 2154;
            auto error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Code_ESPG_horizontal")->QueryUnsignedText(&epsg_code);
            if(error != tinyxml2::XML_SUCCESS)
                std::cout << "Warning: projection system EPSG code not found! It was set to 2154 - i.e. LAMBERT 93 - by default." << std::endl;
            return static_cast<unsigned short>(epsg_code);
        }

        std::map<std::string, scene::BComposition > FileHandler<tinyxml2::XMLDocument>::structure(void) const
        {
            std::map<std::string, scene::BComposition> buildings;

            tinyxml2::XMLElement const* p_building = scene_tree.FirstChildElement("Chantier_Bati3D")
                                                                        ->FirstChildElement("CityModel")
                                                                            ->FirstChildElement("Building");

            while(p_building != NULL)
            {
                /* Building Identifier */
                std::string id(p_building->Attribute("Id"));

                std::set<std::string> roofs;
                std::set<std::string> walls;

                tinyxml2::XMLElement const* p_building_part = p_building->FirstChildElement("BuildingPart");
                while(p_building_part != NULL)
                {
                    tinyxml2::XMLElement const* p_roof = p_building_part->FirstChildElement("RoofSurface");
                    while(p_roof != NULL)
                    {
                        tinyxml2::XMLElement const* p_mesh = p_roof->FirstChildElement("TexturedSurface");
                        while(p_mesh != NULL)
                        {
                            auto roof_name = std::string(p_mesh->Attribute("Id"));
                            if(roof_name.at(0) != 'T')
                                roof_name = "T" + roof_name;
                            roofs.insert(roof_name);
                            p_mesh = p_mesh->NextSiblingElement("TexturedSurface");
                        }
                        p_roof = p_roof->NextSiblingElement("RoofSurface");
                    }

                    tinyxml2::XMLElement const* p_wall = p_building_part->FirstChildElement("WallSurface");
                    while(p_wall != NULL)
                    {
                        tinyxml2::XMLElement const* p_mesh = p_wall->FirstChildElement("TexturedSurface");
                        while(p_mesh != NULL)
                        {
                            auto wall_name = std::string(p_mesh->Attribute("Id"));
                            if(wall_name.at(0) != 'F')
                                wall_name = "T" + wall_name;
                            walls.insert(wall_name);
                            p_mesh = p_mesh->NextSiblingElement("TexturedSurface");
                        }
                        p_wall = p_wall->NextSiblingElement("WallSurface");
                    }
                    p_building_part = p_building_part->NextSiblingElement("BuildingPart");
                }

                buildings.emplace(
                    std::make_pair(
                        id,
                        scene::BComposition(
                            roofs,
                            walls
                        )
                    )
                );

                p_building = p_building->NextSiblingElement("Building");
            }
            return buildings;
        }
    }
}
