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
            UNode(std::string const& building_id, shadow::Point const& _reference_point, unsigned short const _epsg_index, io::FileHandler<Lib3dsFile> const& mesh_file);
            ~UNode(void);

            void swap(UNode & other);
            UNode & operator =(UNode const& other) noexcept;
            UNode & operator =(UNode && other) noexcept;

            /** 
             * Computes the Building bounding box.
             * @return this bounding box
             */
            Bbox_3 bbox(void) const;

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

        void swap(UNode & lhs, UNode & rhs);
    }
}
