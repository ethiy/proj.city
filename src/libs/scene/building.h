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
              Empty Constructor.
              @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              @see Building(Building const& other);
              @see Building(Building&& other);
              @see ~Building(void);
             */
            Building(void);
            /**
              Default Constructor.
              @param _id building identifier
              @param meshes stitched meshes that constitute the building
              @param pivot reference point
              @see Building(void);
              @see Building(Building const& other);
              @see Building(Building&& other);
              @see ~Building(void);
             */
            Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
            /**
              Copy Constructor.
              @param other Building to copy
              @see Building(void);
              @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              @see Building(Building&& other);
              @see ~Building(void);
             */
            Building(Building const& other);
            /**
              Move Constructor.
              @param other Building to move
              @see Building(void);
              @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              @see Building(Building const& other);
              @see ~Building(void);
             */
            Building(Building&& other);
            /**
              Destructor.
              @see Building(void);
              @see Building(std::size_t const& _id, std::vector<urban::shadow::Mesh> const& meshes, shadow::Point const& pivot);
              @see Building(Building const& other);
              @see Building(Building&& other);
             */
            ~Building(void);

            /**
             Swap method.
             @param other Building to be swapped with
             @see void swap(Building & lhs, Building & rhs);
             */
            void swap(Building & other);

            /**
             Copy assignement operator.
             @param other Building to be copyied
             @return the Building copy
             @see Building & operator=(Building && other);
             */
            Building & operator=(Building const& other);
            /**
             Move assignement operator.
             @param other Building to be moved
             @return the moved Building
             @see Building & operator=(Building && other);
             */
            Building & operator=(Building && other);
        private:
            /** identifier */
            std::size_t id;
            /** Bricks */
            std::vector<Brick> bricks;
        };

        /**
         Swap function for Building.
         @param lhs Building to be swapped with rhs
         @param rhs Building to be swapped with lhs
         @see void swap(Building & other);
         */
        void swap(Building & lhs, Building & rhs);
    }
}
