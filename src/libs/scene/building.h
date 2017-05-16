#pragma once

#include "brick/brick.h"

namespace urban
{
    namespace scene
    {
        class Building
        {
        public:
            /**
              * Empty Constructor.
              * @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              * @see Building(Building const& other);
              * @see Building(Building&& other);
              * @see ~Building(void);
             */
            Building(void);
            /**
              * Default Constructor.
              * @param _id building identifier
              * @param meshes stitched meshes that constitute the building
              * @param pivot reference point
              * @param _epsg_code projection system epsg code
              * @see Building(void);
              * @see Building(Building const& other);
              * @see Building(Building&& other);
              * @see ~Building(void);
             */
            Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& _roofs, std::vector<urban::shadow::Mesh> const& _walls, shadow::Point const& _pivot, unsigned short _epsg_code = 2154);
            /**
              * Copy Constructor.
              * @param other Building to copy
              * @see Building(void);
              * @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              * @see Building(Building&& other);
              * @see ~Building(void);
             */
            Building(Building const& other);
            /**
              * Move Constructor.
              * @param other Building to move
              * @see Building(void);
              * @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              * @see Building(Building const& other);
              * @see ~Building(void);
             */
            Building(Building&& other);
            /**
              * Destructor.
              * @see Building(void);
              * @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              * @see Building(Building const& other);
              * @see Building(Building&& other);
             */
            ~Building(void);

            /**
             * Swap method.
             * @param other Building to be swapped with
             * @see void swap(Building & lhs, Building & rhs);
             */
            void swap(Building & other);

            /**
             * Copy assignement operator.
             * @param other Building to be copyied
             * @return the Building copy
             * @see Building & operator=(Building && other);
             */
            Building & operator=(Building const& other);
            /**
             * Move assignement operator.
             * @param other Building to be moved
             * @return the moved Building
             * @see Building & operator=(Building && other);
             */
            Building & operator=(Building && other);

            /** 
             * Access the reference point.
             * @return the reference point
             */
            shadow::Point pivot(void) const noexcept;
            /** 
             * Access the projection system epsg code.
             * @return this projection system epsg code
             */
            unsigned short get_epsg(void) const noexcept;
            /** 
             * Access the id.
             * @return the id
             */
            std::size_t identifier(void) const noexcept;
            /** 
             * Concatenate brick names
             * @return the concatenated names of bricks
             */
            std::string get_name(void) const noexcept;

            /** 
             * Checks if Building is empty.
             * @return if this is empty
             */
            bool is_empty(void) const noexcept;
            
            /** 
             * Computes the Building bounding box.
             * @return this bounding box
             */
            Bbox_3 bbox(void) const;

            std::size_t roofs_size(void) const noexcept;
            std::size_t walls_size(void) const noexcept;
            /** 
             * Access the number of bricks.
             * @return number of bricks
             */
            std::size_t size(void) const noexcept;
            /** Iterator */
            using iterator = std::vector<Brick>::iterator;
            /** Constant Iterator */            
            using const_iterator = std::vector<Brick>::const_iterator;

            /**
             * Returns the iterator at the begining of roofs.
             * @return iterator at the begining.
             */
            iterator roofs_begin(void) noexcept;
            /**
             * Returns the constant begin iterator of roofs.
             * @return constant iterator at the begining.
             */
            const_iterator roofs_begin(void) const noexcept;
            /**
             * Returns the constant begin iterator of roofs.
             * @return constant iterator at the begining.
             */
            const_iterator roofs_cbegin(void) const noexcept;

            /**
             * Returns the begin iterator of roofs.
             * @return iterator at the end.
             */
            iterator roofs_end(void) noexcept;
            /**
             * Returns the constant end iterator of roofs.
             * @return constant iterator at the end.
             */
            const_iterator roofs_end(void) const noexcept;
            /**
             * Returns the constant end iterator of roofs.
             * @return constant iterator at the end.
             */
            const_iterator roofs_cend(void) const noexcept;

            /**
             * Returns the iterator at the begining of walls.
             * @return iterator at the begining.
             */
            iterator walls_begin(void) noexcept;
            /**
             * Returns the constant begin iterator of walls.
             * @return constant iterator at the begining.
             */
            const_iterator walls_begin(void) const noexcept;
            /**
             * Returns the constant begin iterator of walls.
             * @return constant iterator at the begining.
             */
            const_iterator walls_cbegin(void) const noexcept;

            /**
             * Returns the begin iterator of walls.
             * @return iterator at the end.
             */
            iterator walls_end(void) noexcept;
            /**
             * Returns the constant end iterator of walls.
             * @return constant iterator at the end.
             */
            const_iterator walls_end(void) const noexcept;
            /**
             * Returns the constant end iterator of walls.
             * @return constant iterator at the end.
             */
            const_iterator walls_cend(void) const noexcept;
        private:
            /** identifier */
            std::size_t id;
            /** Reference Point */
            shadow::Point reference_point;
            /** Projection system EPSG code*/
            unsigned short epsg_code = 2154;
            /** Roofs */
            std::vector<Brick> roofs;
            /** Walls */
            std::vector<Brick> walls;

            /**
            * Outstreaming the dual adjacency graph
            * @param as the output stream
            * @param building the building to stream
            * @return the output stream
            */
            friend io::Adjacency_stream & operator <<(io::Adjacency_stream & as, Building const& building);
        };

        /**
         Swap function for Building.
         * @param lhs Building to be swapped with rhs
         * @param rhs Building to be swapped with lhs
         * @see void swap(Building & other);
         */
        void swap(Building & lhs, Building & rhs);
    }
}
