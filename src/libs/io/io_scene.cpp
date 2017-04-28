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

    }
}
