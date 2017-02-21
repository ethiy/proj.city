#include "../libs/Shadow/Point/point.h"
#include "../libs/Shadow/Vector/vector.h"
#include "../libs/Shadow/Bbox/bbox.h"

#include <limits>
#include <cmath>

#include <catch.hpp>

SCENARIO("Point manipulation:")
{
    GIVEN("Point coordinates")
    {
        urban::shadow::Point point(15.5343f, -13.4504f, 60.8789f);
        WHEN("the point is created")
        {
            THEN("The output checks:")
            {
                std::ostringstream auxilary, _auxilary;
                auxilary << point.x() << " " << point.y() << " " << point.z();
                _auxilary << point;
                REQUIRE(auxilary.str() == _auxilary.str());
            }
        }
    }

    GIVEN("Two Point coordinates")
    {
        urban::shadow::Point origin(15.5343f, -13.4504f, 60.8789f);
        urban::shadow::Point target(15.7204f, -13.188f, 61.1764f);
        WHEN("the vector is created")
        {
            urban::shadow::Vector v(origin, target);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << v.x() << " " << v.y() << " " << v.z();
                REQUIRE(auxilary.str() == "0.1861 0.262401 0.297501");
            }
        }
    }
        
    GIVEN("Thee Point coordinates")
    {
        urban::shadow::Point first(15.5343f, -13.4504f, 60.8789f);
        urban::shadow::Point second(15.7204f, -13.188f, 60.8789f);
        urban::shadow::Point third(15.7204f, -13.188f, 61.1764f);
        WHEN("one vector is created")
        {
            urban::shadow::Vector v(first, second);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << v.x() << " " << v.y() << " " << v.z();
                REQUIRE(auxilary.str() == "0.1861 0.262401 0");
            }
        }
        WHEN("two points are substructed")
        {
            urban::shadow::Vector u(third - second);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << u.x() << " " << u.y() << " " << u.z();
                REQUIRE(auxilary.str() == "0 0 0.297501");
            }
        }
        WHEN("A point is translated")
        {
            urban::shadow::Vector v(first, second);
            urban::shadow::Point P = first + v;
            THEN("the output checks:")
            {
                REQUIRE(P == second);
            }
        }
        WHEN("two different points are compared")
        {
            THEN("the output checks:")
            {
                REQUIRE(first != second);
            }
        }
        WHEN("An empty bounding box is created")
        {
            urban::shadow::Bbox empty;
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << empty;
                REQUIRE(auxilary.str() == "inf -inf inf -inf inf -inf");
            }
        }
        WHEN("A Bbox is computed for the three points")
        {
            urban::shadow::Bbox bbox = first.bbox() + second.bbox() + third.bbox();
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << bbox;
                REQUIRE(auxilary.str() == "15.5343 15.7204 -13.4504 -13.188 60.8789 61.1764");
            }
        }
        WHEN("the cross product of the two vectors is computed")
        {
            urban::shadow::Vector n(urban::shadow::Vector(first, second) ^ urban::shadow::Vector(second, third));
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << n;
                REQUIRE(auxilary.str() == "0.0780643 -0.0553649 0");
            }
        }
        WHEN("the normal of the plane defined accordingly is computed")
        {
            urban::shadow::Vector n(urban::normal_to(first, second, third));
            urban::shadow::Vector m(urban::shadow::Vector(first, second) ^ urban::shadow::Vector(second, third));
            THEN("the output checks:")
            {
                REQUIRE(n == m / urban::norm_L2(m));
            }
        }
        WHEN("the normal is used in determinant")
        {
            urban::shadow::Vector u = second - first,
                                  v = third - second;
            urban::shadow::Vector n(urban::normal_to(first, second, third));
            THEN("the output checks:")
            {
                REQUIRE(urban::determinant(n, u, v) == n * (u ^ v));
            }
        }
        WHEN("determinant of colinear vectors")
        {
            urban::shadow::Vector u = second - first,
                                  v = third - second;
            THEN("the output checks:")
            {
                REQUIRE(urban::determinant(10. * u, 20000. * u, v) == 0.);
            }
        }
    }
}
