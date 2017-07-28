#include <shadow/Point/point.h>
#include <shadow/Vector/vector.h>
#include <shadow/Bbox/bbox.h>

#include <limits>
#include <cmath>

#include <catch.hpp>

SCENARIO("Point manipulation:")
{
    GIVEN("Point coordinates:")
    {
        urban::shadow::Point point(15.5343, -13.4504, 60.8789);
        WHEN("Out stream created")
        {
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << point;
                REQUIRE(auxilary.str() == "15.5343 -13.4504 60.8789");
            }
        }
    }

    GIVEN("Two Point coordinates")
    {
        urban::shadow::Point origin(15.5343, -13.4504, 60.8789);
        urban::shadow::Point target(15.7204, -13.188, 61.1764);
        WHEN("the vector is created")
        {
            urban::shadow::Vector v(origin, target);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << v.x() << " " << v.y() << " " << v.z();
                REQUIRE(auxilary.str() == "0.1861 0.2624 0.2975");
            }
        }
    }
        
    GIVEN("Thee Point coordinates")
    {
        urban::shadow::Point first(15.5343, -13.4504, 60.8789);
        urban::shadow::Point second(15.7204, -13.188, 60.8789);
        urban::shadow::Point third(15.7204, -13.188, 61.1764);
        WHEN("one vector is created")
        {
            urban::shadow::Vector v(first, second);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << v.x() << " " << v.y() << " " << v.z();
                REQUIRE(auxilary.str() == "0.1861 0.2624 0");
            }
        }
        WHEN("two points are subtracted")
        {
            urban::shadow::Vector u(third - second);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << u.x() << " " << u.y() << " " << u.z();
                REQUIRE(auxilary.str() == "0 0 0.2975");
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
                REQUIRE(auxilary.str() == "0.078064 -0.0553647 0");
            }
        }
        WHEN("the normal of the plane defined accordingly is computed")
        {
            urban::shadow::Vector n(urban::shadow::normal_to(first, second, third));
            urban::shadow::Vector m(urban::shadow::Vector(first, second) ^ urban::shadow::Vector(second, third));
            THEN("the output checks:")
            {
                REQUIRE(n == m / urban::shadow::norm_L2(m));
            }
        }
        WHEN("the normal is used in determinant")
        {
            urban::shadow::Vector u = second - first,
                                  v = third - second;
            urban::shadow::Vector n(urban::shadow::normal_to(first, second, third));
            THEN("the output checks:")
            {
                REQUIRE(std::abs(urban::shadow::determinant(n, u, v) - n * (u ^ v)) < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("determinant of colinear vectors")
        {
            urban::shadow::Vector u = second - first,
                                  v = third - second;
            THEN("the output checks:")
            {
                REQUIRE(urban::shadow::determinant(10. * u, 20000. * u, v) == 0.);
            }
        }
    }
}
