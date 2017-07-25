#pragma once

#include "brick_projection.h"

#include "../../scene/unode.h"

namespace urban
{
    namespace projection
    {
        class FootPrint
        {
        public:
            FootPrint(void);
            FootPrint(scene::UNode const& unode);
            FootPrint(FootPrint const& other);
            FootPrint(FootPrint && other);
            ~FootPrint(void);

            void swap(FootPrint & other);
            FootPrint & operator =(FootPrint const& other);
            FootPrint & operator =(FootPrint && other);

            std::string const& get_name(void) const noexcept;
            shadow::Point const& get_reference_point(void) const noexcept;
            unsigned short get_epsg(void) const noexcept;
            Bbox_2 const& bbox(void) const noexcept;

            using iterator = BrickPrint::iterator;
            using const_iterator = BrickPrint::const_iterator;

            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;
        private:
            std::string name;
            shadow::Point reference_point;
            unsigned short epsg_index = 2154;
            BrickPrint projection;
        };

        void swap(FootPrint & lhs, FootPrint & rhs);
    }
}
