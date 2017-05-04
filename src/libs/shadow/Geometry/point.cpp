#include "point.h"

#include "vector.h"

#include <CGAL/number_utils.h>

namespace urban
{
    namespace shadow
    {
        Point::Point(Lib3dsPoint const& point)
            : Coordinates(point)
        {}
        Point::Point(Point_3 const& point)
            : Coordinates(point)
        {}


        Bbox Point::bbox(void) const
        {
            return Bbox(_coordinates);
        }
    }
    
    shadow::Vector normal_to(shadow::Point const& first, shadow::Point const& second, shadow::Point const& third)
    {
        shadow::Vector v(first, second);
        v ^= shadow::Vector(second, third);
        return v / norm_L2(v);
    }
}
