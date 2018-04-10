#include <io/io_scene.h>

#include <shadow/bbox.h>

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

        scene::Scene FileHandler<tinyxml2::XMLDocument>::read(FileHandler<Lib3dsFile> const& mesh_file) const
        {
            return scene::Scene(mesh_file, pivot(), epsg_index(), building_ids(), terrain_id());
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
        shadow::Point FileHandler<tinyxml2::XMLDocument>::pivot(void) const
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
                shadow::Bbox _bbox = bbox();
                x_offset = (_bbox.xmax() + _bbox.xmin()) / 2.;
                y_offset = (_bbox.ymax() + _bbox.ymin()) / 2.;
                z_offset = (_bbox.zmax() + _bbox.zmin()) / 2.;
            }
            
            return urban::shadow::Point(x_offset, y_offset, z_offset);
        }
        unsigned short FileHandler<tinyxml2::XMLDocument>::epsg_index(void) const
        {
            unsigned int epsg_code = 2154;
            auto error = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("Code_ESPG_horizontal")->QueryUnsignedText(&epsg_code);
            if(error != tinyxml2::XML_SUCCESS)
                std::cout << "Warning: projection system EPSG code not found! It was set to 2154 - i.e. LAMBERT 93 - by default." << std::endl;
            return static_cast<unsigned short>(epsg_code);
        }
        std::vector<std::string> FileHandler<tinyxml2::XMLDocument>::building_ids(void) const
        {
            std::vector<std::string> ids;

            tinyxml2::XMLElement const* p_building = scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("CityModel")->FirstChildElement("Building");
            while(p_building != NULL)
            {
                ids.push_back(std::string(p_building->Attribute("Id")));
                p_building = p_building->NextSiblingElement("Building");
            }

            return ids;
        }
        std::string FileHandler<tinyxml2::XMLDocument>::terrain_id(void) const
        {
            return std::string(
                scene_tree.FirstChildElement("Chantier_Bati3D")->FirstChildElement("CityModel")->FirstChildElement("TINRelief")->Attribute("Id")
            );
        }        
    }
}
