#include "../libs/Projection/Face/face_projection.h"

#include <catch.hpp>

#include <vector>
#include <iterator>

#include <limits>

#include <cmath>

SCENARIO("Face Projection manipulation:")
{
    GIVEN("A perpendicular face")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(1.25, 98.64, 5.))),
                       B(to_exact(urban::InexactKernel::Point_3(1.25, 98.64, 0.))),
                       C(to_exact(urban::InexactKernel::Point_3(87.3, .029, 3.615)));
        std::vector<urban::Point_2> vertices;
        vertices.push_back(urban::Point_2(A.x(), A.y()));
        vertices.push_back(urban::Point_2(C.x(), C.y()));

        urban::projection::FacePrint perpendicular(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                )
            ),
            urban::Plane_3(A, B, C)
        );

        WHEN("the perpendicularity is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(perpendicular.is_perpendicular());
            }
        }

        WHEN("the area is computed")
        {
            THEN("the output checks:")
            {
                REQUIRE(perpendicular.area() < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("the degeneracy is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(perpendicular.is_degenerate());
            }
        }
        WHEN("A point on the border is tested")
        {
            urban::Point_2 P(urban::Point_2(C.x(), C.y()) + .2 * (urban::Point_2(A.x(), A.y()) - urban::Point_2(C.x(), C.y())));
            THEN("the test checks:")
            {
                REQUIRE(perpendicular.contains(P));
            }
        }
        WHEN("A points height is computed")
        {
            THEN("An exception is thrown:")
            {
                REQUIRE_THROWS_AS(perpendicular.get_height(urban::Point_2(0, 0)), std::overflow_error);
            }
        }
        WHEN("A point on the exterior is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(!perpendicular.contains(urban::Point_2(0, 0)));
            }
        }
    }
}
