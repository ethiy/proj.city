#pragma once

#include "../geometry_definitions.h"
#include "../brick/brick.h"
#include "../shadow/mesh.h"

#include <string>
#include <vector>

namespace urban
{
    class Unit
    {
    public:
        /**
         * Void Constructor
         * @see Unit(const shadow::Point & _reference_point, unsigned char _epsg_index, const std::vector<Brick> & _bricks)
         * @see Unit(const Unit & other)
         * @see Unit(Unit && other)
         * @see ~Unit()
         */
        Unit(void);
        /**
         * CLassic constructor
         * @param _reference_point reference point
         * @param _epsg_index epsg index of the geodesic projection
         * @param _bricks bricks making a unit
         * @see Unit()
         * @see Unit(const Unit & other)
         * @see Unit(Unit && other)
         * @see ~Unit()
         **/
        Unit(const shadow::Point & _reference_point, unsigned char _epsg_index, const std::vector<Brick> & _bricks);
        /**
         * Copy constructor
         * @param other other Unit
         * @see Unit()
         * @see Unit(const shadow::Point & _reference_point, unsigned char _epsg_index, const std::vector<Brick> & _bricks)
         * @see Unit(Unit && other)
         * @see ~Unit()
         **/
        Unit(const Unit & other);
        /**
         * Move constructor
         * @param other other Unit rvalue
         * @see Unit()
         * @see Unit(const shadow::Point & _reference_point, unsigned char _epsg_index, const std::vector<Brick> & _bricks)
         * @see Unit(const Unit & other)
         * @see ~Unit()
         **/
        Unit(Unit && other);
        /**
         * Distructor
         * @see Unit()
         * @see Unit(const shadow::Point & _reference_point, unsigned char _epsg_index, const std::vector<Brick> & _bricks)
         * @see Unit(const Unit & other)
         * @see Unit(Unit && other)
         */
        ~Unit(void);


        /**
         * Swap utility
         * @param other other Unit
         */
        void swap(Unit & other);

        /**
         * Copy assignement
         * @param other Unit
         * @return this by reference
         * @see operator=(const Unit & other)
         */
        Unit & operator=(const Unit & other) noexcept;
        /**
         * Move assignement
         * @param other Unit rvalue
         * @return this by reference
         * @see operator=(const Unit & other)
         */
        Unit & operator=(const Unit & other) noexcept;

        /**
         * Computes the bounding box of the Unit
         * @return the 3D Bounding box
         */
         Bbox_3 bbox(void);

    private:
        std::string name;
        shadow::Point reference_point;
        unsigned char epsg_index = 2154;
        Polyhedron surface;
    };

    void swap(Unit & lhs, Unit & rhs);
}
