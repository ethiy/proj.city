#include "io_scene.h"

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

        scene::Scene FileHandler<tinyxml2::XMLDocument>::read(void) const
        {
            return scene::Scene(pivot(), epsg_code(), structure());
        }

        shadow::Point FileHandler<tinyxml2::XMLDocument>::pivot(void) const
        {
            double  x_offset(0),
                    y_offset(0),
                    z_offset(0);
            
            auto error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_x")->QueryDoubleText(&x_offset);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read Pivot Point");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_y")->QueryDoubleText(&y_offset);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read Pivot Point");
            error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Pivot")->FirstChildElement("offset_z")->QueryDoubleText(&z_offset);
            if(error != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("Could not read Pivot Point");
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

        std::map<std::size_t, scene::BStructure > FileHandler<tinyxml2::XMLDocument>::structure(void) const
        {
            std::map<std::size_t, std::pair< std::set<std::string>, std::set<std::string> > > buildings;

            tinyxml2::XMLElement const* p_building = scene_tree.FirstChildElement("Chantier_Bati3D")
                                                                        ->FirstChildElement("CityModel")
                                                                            ->FirstChildElement("Building");

            while(p_building != NULL)
            {
                /* Building Identifier */
                unsigned int id = 0;
                p_building->QueryUnsignedAttribute("Id", &id);


                std::set<std::string> roofs;
                tinyxml2::XMLElement const* p_roof = p_building->FirstChildElement("BuildingPart")->FirstChildElement("RoofSurface");
                while(p_roof != NULL)
                {
                    tinyxml2::XMLElement const* p_mesh = p_roof->FirstChildElement("TexturedSurface");
                    while(p_mesh != NULL)
                    {
                        roofs.insert(std::string(p_mesh->Attribute("Id")));
                        p_mesh = p_mesh->NextSiblingElement("TexturedSurface");
                    }
                    p_roof = p_roof->NextSiblingElement("RoofSurface");
                }


                std::set<std::string> walls;
                tinyxml2::XMLElement const* p_wall = p_building->FirstChildElement("BuildingPart")->FirstChildElement("WallSurface");
                while(p_wall != NULL)
                {
                    tinyxml2::XMLElement const* p_mesh = p_wall->FirstChildElement("TexturedSurface");
                    while(p_mesh != NULL)
                    {
                        walls.insert(std::string(p_mesh->Attribute("Id")));
                        p_mesh = p_mesh->NextSiblingElement("TexturedSurface");
                    }
                    p_wall = p_wall->NextSiblingElement("WallSurface");
                }
                buildings.emplace(
                    std::make_pair(
                        id,
                        scene::BStructure(
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
