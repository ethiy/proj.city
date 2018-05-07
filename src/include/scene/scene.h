#pragma once

#include <shadow/mesh.h>

#include <scene/unode.h>

#include <string>
#include <map>
#include <set>

namespace city
{
    namespace scene
    {
        class Scene
        {
        public:
            /**
             * Empty Constructor.
             * @see Scene(city::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(void);
            Scene(
                std::vector<shadow::Mesh> const& building_meshes,
                shadow::Mesh const& terrain_mesh,
                city::shadow::Point const& _pivot = shadow::Point(),
                unsigned short _epsg_index = 2154
            );
            /**
             * Copy Constructor.
             * @param other Scene to copy
             * @see Scene(void);
             * @see Scene(city::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(Scene const& other);
            /**
             * Move Constructor.
             * @param other Scene to move
             * @see Scene(void);
             * @see Scene(city::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see ~Scene(void);
             */
            Scene(Scene && other);
            /**
             * Destructor.
             * @see Scene(void);
             * @see Scene(city::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
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
             * Copy assignment operator.
             * @param other Scene to copy
             * @return copy of other
             * @see Scene & operator=(Scene && other);
             */
            Scene & operator =(Scene const& other);
            /**
             * Move assignment operator.
             * @param other Scene to move
             * @return moved Scene
             * @see Scene & operator=(Scene const& other);
             */
            Scene & operator =(Scene && other);

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
             * Access the terrain surface
             * @return a const reference to the terrain surface
             */
            UNode const& get_terrain(void) const noexcept;
            
            std::vector<std::string> identifiers(void) const;

            using iterator = std::vector<UNode>::iterator;
            using const_iterator = std::vector<UNode>::const_iterator;

            iterator begin(void) noexcept;
            iterator end(void) noexcept;
            const_iterator begin(void) const noexcept;
            const_iterator end(void) const noexcept;
            const_iterator cbegin(void) const noexcept;
            const_iterator cend(void) const noexcept;

            std::size_t size(void) const noexcept;

            Scene & prune(bool const terrain);
        private:
            /** Pivot */
            city::shadow::Point pivot;
            /** EPSG projection system code */
            unsigned short epsg_index = 2154;
            /** Scene Buildings */
            std::vector<UNode> buildings;
            /** Scene terrain */
            UNode terrain;
        };

        /**
         Swap method.
         @param lhs Scene to swap with rhs
         @param rhs Scene to swap with lhs
         @see void swap(Scene & other);
         */
        void swap(Scene & lhs, Scene & rhs);
    }

    scene::Scene & prune(scene::Scene & scene, bool const terrain = false);
}
