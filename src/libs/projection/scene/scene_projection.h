#pragma once

#include "brick_projection.h"

#include "../scene/unode.h"

namespace urban
{
    namespace projection
    {
        class FootPrint
        {
        public:
            FootPrint(UNode const& unode);
            FootPrint(FootPrint const& other);
            FootPrint(FootPrint && other);
            ~FootPrint(void);

            void swap(FootPrint & other);
            FootPrint & operator =(FootPrint const& other);
            FootPrint & operator =(FootPrint && other);
        private:
            std::string name;
            shadow::Point reference_point;
            unsigned short epsg_index = 2154;
            BrickPrint projection;
        };

        void swap(FootPrint & lhs, FootPrint & rhs);
    }
}
