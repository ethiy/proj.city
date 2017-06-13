#pragma once

#include "../geometry_definitions.h"

#include "../shadow/Point/point.h"

#include "../io/io_3ds.h"

#include <string>

namespace urban
{
    namespace scene
    {
        class UNode
        {
        public:
            UNode(void);
            UNode(UNode const& other);
            UNode(UNode && other);
            UNode(std::string const& building_id, io::FileHandler<Lib3dsFile> const& mesh_file);
            ~UNode(void);

            void swap(UNode & other);
            UNode & operator =(UNode const& other) noexcept;
            UNode & operator =(UNode && other) noexcept;
        private:
            /** Node name */
            std::string name;
            /** Reference Point */
            shadow::Point reference_point;
            /** Projection system EPSG code*/
            unsigned short epsg_index = 2154;
            /** The 3D surface*/
            Polyhedron surface;
            /** Bounding box*/
            Bbox_3 bounding_box;
        };
    }
}
