#pragma once

#include "../shadow/mesh.h"

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
            Scene(urban::shadow::Point const& _pivot, unsigned short _epsg_code, std::map<std::size_t, BComposition > const& _structure);
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
            Scene & operator =(Scene const& other);
            /**
             * Move assignement operator.
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

            std::vector<std::size_t> identifiers(void) const;

            std::vector<urban::shadow::Mesh> roofs(std::size_t identifier, std::vector<urban::shadow::Mesh> const& meshes) const;
            std::vector<urban::shadow::Mesh> roofs(std::size_t identifier, std::map<std::string, urban::shadow::Mesh> const& ordered_meshes) const;
            std::vector<urban::shadow::Mesh> walls(std::size_t identifier, std::vector<urban::shadow::Mesh> const& meshes) const;
            std::vector<urban::shadow::Mesh> walls(std::size_t identifier, std::map<std::string, urban::shadow::Mesh> const& ordered_meshes) const;

            /**
             * * Structure a vector of meshes
             * @param meshes a vector of meshes to structure
             * @return a map of mesh vectors structured according to the structure
             */
            std::map<std::size_t, std::pair<std::vector<urban::shadow::Mesh>, std::vector<urban::shadow::Mesh> > > cluster(std::vector<shadow::Mesh> const& meshes) const;
        private:
            /** Pivot */
            urban::shadow::Point pivot;
            /** EPSG projection system code */
            unsigned short epsg_code = 2154;
            /** Scene Structure */
            std::map<std::size_t, BComposition > structure;
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
