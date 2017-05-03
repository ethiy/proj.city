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
            Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot, unsigned short _epsg_code = 2154);
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
             * Access the id.
             * @return the id
             */
            std::size_t identifier(void) const noexcept;

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
             * Returns the end iterator.
             * @return iterator at the begining.
             */
            iterator begin(void) noexcept;
            /**
             * Returns the constant begin iterator.
             * @return constant iterator at the begining.
             */
            const_iterator begin(void) const noexcept;
            /**
             * Returns the constant begin iterator.
             * @return constant iterator at the begining.
             */
            const_iterator cbegin(void) const noexcept;

            /**
             * Returns the begin iterator.
             * @return iterator at the end.
             */
            iterator end(void) noexcept;
            /**
             * Returns the constant end iterator.
             * @return constant iterator at the end.
             */
            const_iterator end(void) const noexcept;
            /**
             * Returns the constant end iterator.
             * @return constant iterator at the end.
             */
            const_iterator cend(void) const noexcept;
        private:
            /** identifier */
            std::size_t id;
            /** Reference Point */
            shadow::Point reference_point;
            /** Projection system EPSG code*/
            unsigned short epsg_code = 2154;
            /** Bricks */
            std::vector<Brick> bricks;
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
