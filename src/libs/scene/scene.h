#pragma once

#include "../shadow/mesh.h"

#include "unode.h"

#include <string>
#include <map>
#include <set>

namespace urban
{
    namespace scene
    {
        struct BComposition
        {
            BComposition(std::set<std::string> _roofs, std::set<std::string> _walls)
                : roofs(_roofs), walls(_walls)
            {}
            BComposition(BComposition const& other)
                : roofs(other.roofs), walls(other.walls)
            {}
            BComposition(BComposition && other)
                : roofs(std::move(other.roofs)), walls(std::move(other.walls))
            {}
            ~BComposition(void) {}

            BComposition & operator =(BComposition const& other);
            BComposition & operator =(BComposition && other);

            std::set<std::string> roofs;
            std::set<std::string> walls;
        };

        void swap(BComposition & lhs, BComposition & rhs);

        class Scene
        {
        public:
            /**
             * Empty Constructor.
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(void);
            Scene(urban::shadow::Point const& _pivot, bool _centered, unsigned short _epsg_index, std::vector<std::string> const& building_ids, io::FileHandler<Lib3dsFile> const& mesh_file);
            /**
             * Copy Constructor.
             * @param other Scene to copy
             * @see Scene(void);
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
             * @see Scene(Scene && other);
             * @see ~Scene(void);
             */
            Scene(Scene const& other);
            /**
             * Move Constructor.
             * @param other Scene to move
             * @see Scene(void);
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
             * @see Scene(Scene const& other);
             * @see ~Scene(void);
             */
            Scene(Scene && other);
            /**
             * Destructor.
             * @see Scene(void);
             * @see Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_index, std::map<std::string, std::set<std::string> > const& _structure);
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

            std::vector<std::string> identifiers(void) const;
        private:
            /** Pivot */
            urban::shadow::Point pivot;
            /** Centered */
            bool centered = true;
            /** EPSG projection system code */
            unsigned short epsg_index = 2154;
            /** Scene Buildings */
            std::vector<UNode> buildings;
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
