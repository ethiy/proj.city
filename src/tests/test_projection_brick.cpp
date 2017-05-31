#include <projection/Brick/brick_projection.h>

#include <catch.hpp>

#include <vector>

#include <ostream>
#include <sstream>

SCENARIO("Brick projection manipulation")
{
    GIVEN("A Face projection")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(64.25, 50., 0.))),
                    B(to_exact(urban::InexactKernel::Point_3(1.25, 98.64, 5.))),
                    C(to_exact(urban::InexactKernel::Point_3(87.3, .029, 3.615)));
        std::vector<urban::Point_2> vertices{{
            urban::Point_2(A.x(), A.y()),
            urban::Point_2(B.x(), B.y()),
            urban::Point_2(C.x(), C.y())
        }};

        urban::projection::FacePrint facet(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                )
            ),
            urban::Plane_3(A, B, C)
        );
        urban::projection::BrickPrint face_proj(facet);

        WHEN("it is encapsulated into a brick projection:")
        {
            THEN("The output checks:")
            {
                std::stringstream auxilary;
                auxilary << face_proj;
                REQUIRE(auxilary.str() == "Name: contains_only_one_facet\nBounding box: 1.25 0.029 87.3 98.64\nReference Point: 0 0 0\nEPSG index: 2154\nFace Projections: 1\nThe Polygon describing borders :3 64.25 50 1.25 98.64 87.3 0.029  0 \nThe supporting plane coefficients : 425.689 342.995 2027.02 -44500.2\n\nProjected surface: \n3 1.25 98.64 87.3 0.029 64.25 50  0 \n");
            }
        }
        WHEN("it is compared to an encapsulated brick projection:")
        {
            THEN("The output checks:")
            {
                REQUIRE((face_proj.contains(facet) && face_proj.overlaps(facet) && face_proj.is_under(facet)));
            }
        }
    }
    GIVEN("A Brick Print and a facet")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(-1., 0, 5.))),
                       B(to_exact(urban::InexactKernel::Point_3(1., 0., 2.))),
                       C(to_exact(urban::InexactKernel::Point_3(0, 1., 3.615)));
        urban::Point_3 _A(to_exact(urban::InexactKernel::Point_3(-.5, .33, 5.))),
                       _B(to_exact(urban::InexactKernel::Point_3(.5, .33, 5.))),
                       _C(to_exact(urban::InexactKernel::Point_3(0, .67, 8.2)));

        std::vector<urban::Point_2> vertices{{
            urban::Point_2(A.x(), A.y()),
            urban::Point_2(B.x(), B.y()),
            urban::Point_2(C.x(), C.y())
        }};
        
        std::vector<urban::Point_2> _vertices{{
            urban::Point_2(_A.x(), _A.y()),
            urban::Point_2(_B.x(), _B.y()),
            urban::Point_2(_C.x(), _C.y())
        }};

        urban::projection::FacePrint face_1(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                )
            ),
            urban::Plane_3(A, B, C)
        );
        urban::projection::FacePrint face_2(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(_vertices),
                    std::end(_vertices)
                )
            ),
            urban::Plane_3(_A, _B, _C)
        );

        WHEN("it is encapsulated into a brick projection:")
        {
            urban::projection::BrickPrint proj(face_2);
            THEN("The output checks:")
            {
                std::stringstream auxilary;
                auxilary << proj;
                REQUIRE(auxilary.str() == "Name: contains_only_one_facet\nBounding box: -0.5 0.33 0.5 0.67\nReference Point: 0 0 0\nEPSG index: 2154\nFace Projections: 1\nThe Polygon describing borders :3 -0.5 0.33 0.5 0.33 0 0.67  0 \nThe supporting plane coefficients : 0 -3.2 0.34 -0.644\n\nProjected surface: \n3 -0.5 0.33 0.5 0.33 0 0.67  0 \n");
            }
        }
        WHEN("face_1 is compared to the other")
        {
            urban::projection::BrickPrint proj(face_2);
            THEN("The output checks:")
            {
                REQUIRE((!proj.contains(face_1) && proj.overlaps(face_1)));
            }
        }
        WHEN("face_2 is compared to the other")
        {
            urban::projection::BrickPrint proj(face_1);
            THEN("The output checks:")
            {
                REQUIRE((proj.contains(face_2) && proj.overlaps(face_2) && !proj.is_under(face_2)));
            }
        }
        WHEN("face_2 is added to the other")
        {
            urban::projection::BrickPrint proj(face_1);
            proj.insert(face_2);
            THEN("The output checks:")
            {
                std::stringstream auxilary;
                auxilary << proj;
                REQUIRE(auxilary.str() == "Name: contains_only_one_facet\nBounding box: -1 0 1 1\nReference Point: 0 0 0\nEPSG index: 2154\nFace Projections: 2\nThe Polygon describing borders :3 -1 0 1 0 0 1  1 3 0.5 0.33 -0.5 0.33 0 0.67  \nThe supporting plane coefficients : 3 -0.23 2 -7\n\nThe Polygon describing borders :3 -0.5 0.33 0.5 0.33 0 0.67  0 \nThe supporting plane coefficients : 0 -3.2 0.34 -0.644\n\nProjected surface: \n3 -1 0 1 0 0 1  0 \n");
            }
        }
    }

    GIVEN("two non convexe facets")
    {
        std::vector<urban::Point_2> vertices{{
            urban::Point_2(-10, 6),
            urban::Point_2(-12, 0),
            urban::Point_2(0, 0),
            urban::Point_2(-6, -12),
            urban::Point_2(2, -12),
            urban::Point_2(2, 8),
            urban::Point_2(-2, 4)
        }};
        
        urban::projection::FacePrint face_1(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                )
            ),
            urban::Plane_3(
                urban::Point_3(-10, 6, 0),
                urban::Point_3(-12, 0, 0),
                urban::Point_3(0, 0, 0)                
            )
        );

        vertices.clear();
        vertices = std::vector<urban::Point_2>{{
            urban::Point_2(-6, 4),
            urban::Point_2(-4, 2),
            urban::Point_2(-6, 2)
        }};

        std::list<urban::Polygon> hole_list{{urban::Polygon(std::begin(vertices), std::end(vertices))}};

        vertices.clear();
        vertices = std::vector<urban::Point_2>{{
            urban::Point_2(-6, 8),
            urban::Point_2(-10, -10),
            urban::Point_2(2, -10),
            urban::Point_2(-2, -2),
            urban::Point_2(-4, 6),
            urban::Point_2(4, 4)
        }};

        urban::projection::FacePrint face_2(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                ),
                std::begin(hole_list),
                std::end(hole_list)
            ),
            urban::Plane_3(
                urban::Point_3(-6, 8, 10),
                urban::Point_3(-10, 10, 10),
                urban::Point_3(2, -10, 10)                
            )
        );
        WHEN("face_2 is compared to the other")
        {
            urban::projection::BrickPrint proj(face_1);
            THEN("The output checks:")
            {
                REQUIRE((!proj.contains(face_2) && proj.overlaps(face_2) && !proj.is_under(face_2)));
            }
        }
    }
}
