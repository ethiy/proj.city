#include "../libs/projection/Face/face_projection.h"

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

    GIVEN("A degenerate face")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(64.25, 50., 0.))),
                       B(to_exact(urban::InexactKernel::Point_3(1.25, 98.64, 5.))),
                       C(to_exact(urban::InexactKernel::Point_3(87.3, .029, 3.615)));
        std::vector<urban::Point_2> vertices;
        vertices.push_back(urban::Point_2(A.x(), A.y()));
        vertices.push_back(urban::Point_2(B.x(), B.y()));
        vertices.push_back(urban::Point_2(C.x(), C.y()));
        std::list<urban::Polygon> l{urban::Polygon(std::begin(vertices), std::end(vertices))};

        urban::projection::FacePrint degenerate(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                ),
                std::begin(l),
                std::end(l)
            ),
            urban::Plane_3(A, B, C)
        );
        WHEN("the perpendicularity is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(!degenerate.is_perpendicular());
            }
        }

        WHEN("the area is computed")
        {
            THEN("the output checks:")
            {
                REQUIRE(degenerate.area() < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("the degeneracy is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(degenerate.is_degenerate());
            }
        }
        WHEN("A point on the border is tested")
        {
            urban::Point_2 P(urban::Point_2(C.x(), C.y()) + .2 * (urban::Point_2(A.x(), A.y()) - urban::Point_2(C.x(), C.y())));
            THEN("the test checks:")
            {
                REQUIRE(degenerate.contains(P));
            }
        }
        WHEN("the height of a point outside the surface is computed")
        {
            THEN("An exception is thrown:")
            {
                REQUIRE(std::abs(degenerate.get_height(urban::Point_2(0, 0))) < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("the plane height of a point outside the surface is computed")
        {
            THEN("An exception is thrown:")
            {
                REQUIRE(std::abs(degenerate.get_plane_height(urban::Point_2(0, 0)) - 21.9535182665) < std::numeric_limits<float>::epsilon());
            }
        }
        WHEN("A point on the exterior is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(!degenerate.contains(urban::Point_2(0, 0)));
            }
        }
    }
    GIVEN("A triangular face with a triangular hole")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(-3, -5, 5.))),
                       B(to_exact(urban::InexactKernel::Point_3(3, -5, 5.))),
                       C(to_exact(urban::InexactKernel::Point_3(3, 5, 0.))),
                       D(to_exact(urban::InexactKernel::Point_3(-3, 5, .0)));

        std::vector<urban::Point_2> vertices;
        vertices.push_back(urban::Point_2(A.x(), A.y()));
        vertices.push_back(urban::Point_2(B.x(), B.y()));
        vertices.push_back(urban::Point_2(C.x(), C.y()));
        vertices.push_back(urban::Point_2(D.x(), D.y()));

        std::vector<urban::Point_2> hole_vertices;
        hole_vertices.push_back(urban::Point_2(-1, -3));
        hole_vertices.push_back(urban::Point_2(-1, 3));
        hole_vertices.push_back(urban::Point_2(1, 3));
        hole_vertices.push_back(urban::Point_2(1, -3));

        std::list<urban::Polygon> l{urban::Polygon(std::begin(hole_vertices), std::end(hole_vertices))};

        urban::projection::FacePrint example(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                ),
                std::begin(l),
                std::end(l)
            ),
            urban::Plane_3(A, B, C)
        );
        WHEN("the perpendicularity is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(!example.is_perpendicular());
            }
        }

        WHEN("the area is computed")
        {
            THEN("the output checks:")
            {
                REQUIRE(std::abs(example.area() - 72) < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("the degeneracy is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(!example.is_degenerate());
            }
        }
        WHEN("A point on the border is tested")
        {
            urban::Point_2 P(urban::Point_2(C.x(), C.y()) + .2 * (urban::Point_2(A.x(), A.y()) - urban::Point_2(C.x(), C.y())));
            THEN("the test checks:")
            {
                REQUIRE(example.contains(P));
            }
        }
        WHEN("the height of a point outside the surface is computed")
        {
            THEN("An exception is thrown:")
            {
                REQUIRE(std::abs(example.get_height(urban::Point_2(0, 0))) < std::numeric_limits<double>::epsilon());
            }
        }
        WHEN("the plane height of a point outside the surface is computed")
        {
            THEN("An exception is thrown:")
            {
                REQUIRE(std::abs(example.get_plane_height(urban::Point_2(0, 0)) - 2.5) < std::numeric_limits<float>::epsilon());
            }
        }
        WHEN("A point on the exterior is tested")
        {
            THEN("the test checks:")
            {
                REQUIRE(!example.contains(urban::Point_2(0, 0)));
            }
        }
    }
}
