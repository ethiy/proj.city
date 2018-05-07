#include <shadow/point.h>
#include <shadow/vector.h>
#include <shadow/bbox.h>

#include <limits>
#include <cmath>

#include <catch.hpp>

SCENARIO("Point manipulation:")
{
    GIVEN("Point coordinates:")
    {
        city::shadow::Point point(15.5343, -13.4504, 60.8789);
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
        city::shadow::Point origin(15.5343, -13.4504, 60.8789);
        city::shadow::Point target(15.7204, -13.188, 61.1764);
        WHEN("the vector is created")
        {
            city::shadow::Vector v(origin, target);
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
        city::shadow::Point first(15.5343, -13.4504, 60.8789);
        city::shadow::Point second(15.7204, -13.188, 60.8789);
        city::shadow::Point third(15.7204, -13.188, 61.1764);
        WHEN("one vector is created")
        {
            city::shadow::Vector v(first, second);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << v.x() << " " << v.y() << " " << v.z();
                REQUIRE(auxilary.str() == "0.1861 0.2624 0");
            }
        }
        WHEN("two points are subtracted")
        {
            city::shadow::Vector u(third - second);
            THEN("The output checks:")
            {
                std::ostringstream auxilary;
                auxilary << u.x() << " " << u.y() << " " << u.z();
                REQUIRE(auxilary.str() == "0 0 0.2975");
            }
        }
        WHEN("A point is translated")
        {
            city::shadow::Vector v(first, second);
            city::shadow::Point P = first + v;
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
            city::shadow::Bbox empty;
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << empty;
                REQUIRE(auxilary.str() == "inf -inf inf -inf inf -inf");
            }
        }
        WHEN("A Bbox is computed for the three points")
        {
            city::shadow::Bbox bbox = first.bbox() + second.bbox() + third.bbox();
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << bbox;
                REQUIRE(auxilary.str() == "15.5343 15.7204 -13.4504 -13.188 60.8789 61.1764");
            }
        }
        WHEN("the cross product of the two vectors is computed")
        {
            city::shadow::Vector n(city::shadow::Vector(first, second) ^ city::shadow::Vector(second, third));
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << n;
                REQUIRE(auxilary.str() == "0.078064 -0.0553647 0");
            }
        }
        WHEN("the normal of the plane defined accordingly is computed")
        {
            city::shadow::Vector n(city::shadow::normal_to(first, second, third));
            city::shadow::Vector m(city::shadow::Vector(first, second) ^ city::shadow::Vector(second, third));
            THEN("the output checks:")
            {
                REQUIRE(n == m / city::shadow::norm_L2(m));
            }
        }
        WHEN("the normal is used in determinant")
        {
            city::shadow::Vector u = second - first,
                                  v = third - second;
            city::shadow::Vector n(city::shadow::normal_to(first, second, third));
            THEN("the output checks:")
            {
                REQUIRE(std::abs(city::shadow::determinant(n, u, v) - n * (u ^ v)) < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("determinant of colinear vectors")
        {
            city::shadow::Vector u = second - first,
                                  v = third - second;
            THEN("the output checks:")
            {
                REQUIRE(city::shadow::determinant(10. * u, 20000. * u, v) == 0.);
            }
        }
    }
}
