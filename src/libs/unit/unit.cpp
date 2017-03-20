#include "unit.h"

#include <iterator>
#include <>

namespace urban
{
    Unit::Unit(void) {}
    Unit::Unit(const shadow::Point & _reference_point, unsigned char _espg_index, const std::vector<Brick> & _bricks): reference_point(_reference_point), espg_index(_espg_index), bricks(_bricks) {}
    Unit::Unit(const Unit & other): name(other.name), reference_point(other.reference_point), espg_index(other.espg_index), bricks(other.bricks) {}
    Unit::Unit(const Unit & other): name(std::move(other.name)), reference_point(std::move(other.reference_point)), espg_index(std::move(other.espg_index)), bricks(std::move(other.bricks)) {}
    Unit::~Unit(void) {}

    void Unit::swap(Unit & other)
    {
        using std::swap;
        swap(name, other.name);
        swap(reference_point, other.reference_point);
        swap(espg_index, other.espg_index);
        swap(bricks, other.bricks);
    }

    Unit & Unit::operator=(const Unit & other) noexcept
    {
        name = other.name;
        reference_point = other.reference_point;
        espg_index = other.espg_index;
        bricks = other.bricks;
        return *this;
    }

    Unit & Unit::operator=(Unit && other) noexcept
    {
        name = std::move(other.name);
        reference_point = std::move(other.reference_point);
        espg_index = std::move(other.espg_index);
        bricks = std::move(other.bricks);
        return *this;
    }

    Bbox_3 Unit::bbox(void)
    {
        return std::accumulate(

        );
    }

    void swap(Unit & lhs, Unit & rhs)
    {
        lhs.swap(rhs);
    }
}
