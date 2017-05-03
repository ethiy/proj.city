#pragma once

#include "../shadow/mesh.h"

#include <string>
#include <map>
#include <set>

namespace urban
{
    namespace scene
    {
        class Scene
        {
        public:
            /**
             * Empty Constructor.
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(void);
            /**
             * Default Constructor.
             * @param _pivot pivot point
             * @param _epsg_code EPSG projection system code
             * @param _structure scene structure
             * @see Scene(void);
             * @see Scene(Scene const& other);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, std::set<std::string> > const& _structure);
            /**
             * Copy Constructor.
             * @param other Scene to copy
             * @see Scene(void);
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, std::set<std::string> > const& _structure);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(Scene const& other);
            /**
             * Move Constructor.
             * @param other Scene to move
             * @see Scene(void);
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see ~Scene(void);
             */
            Scene(Scene && other);
            /**
             * Destructor.
             * @see Scene(void);
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see Scene(Scene && other);
             */
            ~Scene(void);

            /**
             * Swap method.
             * @param other Scene to swap with
             * @see void swap(Scene & lhs, Scene & rhs);
             */
            void swap(Scene & other);

            /**
             * Copy assignement operator.
             * @param other Scene to copy
             * @return copy of other
             * @see Scene & operator=(Scene && other);
             */
            Scene & operator=(Scene const& other);
            /**
             * Move assignement operator.
             * @param other Scene to move
             * @return moved Scene
             * @see Scene & operator=(Scene const& other);
             */
            Scene & operator=(Scene && other);

            /**
             * Access pivot point
             * @return pivot points
             */
            shadow::Point get_pivot(void) const noexcept;
            /**
             * Access the EPSG projection system code
             * @return the EPSG projection system code
             */
            unsigned short get_epsg(void) const noexcept;

            /**
             * * Structure a vector of meshes
             * @param meshes a vector of meshes to structure
             * @return a map of mesh vectors structured according to the structure
             */
            std::map<std::size_t, std::vector<urban::shadow::Mesh> > cluster(std::vector<shadow::Mesh> const& meshes) const;
        private:
            /** Pivot */
            urban::shadow::Point pivot;
            /** EPSG projection system code */
            unsigned short epsg_code = 2154;
            /** Scene Structure */
            std::map<std::size_t, std::set<std::string> > structure;
        };

        /**
         Swap method.
         @param lhs Scene to swap with rhs
         @param rhs Scene to swap with lhs
         @see void swap(Scene & other);
         */
        void swap(Scene & lhs, Scene & rhs);
    }
}
