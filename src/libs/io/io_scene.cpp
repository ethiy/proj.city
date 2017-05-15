#include "io_scene.h"

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
                std::cout << "Warining: projection system EPSG code not found! Set to LAMBERT 93 : 2154 by default." << std::endl;
            return static_cast<unsigned short>(epsg_code);
        }

        std::map<std::size_t, std::set<std::string> > FileHandler<tinyxml2::XMLDocument>::buildings(void) const
        {
            std::map<std::size_t, std::set<std::string> > buildings;

            tinyxml2::XMLElement const* p_buildings = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("CityModel")->FirstChildElement("Building");
            while(p_buildings != NULL)
            {
                unsigned int id = 0;
                std::set<std::string> building_parts;
                p_buildings->QueryUnsignedAttribute("Id", &id);
                tinyxml2::XMLElement const* p_surface = p_buildings->FirstChildElement("BuildingPart")->FirstChildElement("RoofSurface");
                while(p_surface != NULL)
                {
                    tinyxml2::XMLElement const* p_mesh = p_surface->FirstChildElement("TexturedSurface");
                    while(p_mesh != NULL)
                    {
                        building_parts.insert(std::string(p_mesh->Attribute("Id")));
                        p_mesh = p_mesh->NextSiblingElement("TexturedSurface");
                    }
                    p_surface = p_surface->NextSiblingElement("WallSurface");
                }
                buildings.emplace(std::make_pair(id, building_parts));
                p_buildings = p_buildings->NextSiblingElement("Building");
            }
            return buildings;
        }
    }
}
