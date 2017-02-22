#include "../libs/Algorithms/algorithms.h"
#include <CGAL/Boolean_set_operations_2.h>

#include <catch.hpp>

#include <vector>
#include <iterator>
#include <list>

SCENARIO("Occlusion management")
{
    GIVEN("two triangular faces")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(-1., 0, 5.))),
                       B(to_exact(urban::InexactKernel::Point_3(1., 0., 2.))),
                       C(to_exact(urban::InexactKernel::Point_3(0, 1., 3.615)));
        urban::Point_3 _A(to_exact(urban::InexactKernel::Point_3(-.5, .33, 5.))),
                       _B(to_exact(urban::InexactKernel::Point_3(.5, .33, 5.))),
                       _C(to_exact(urban::InexactKernel::Point_3(0, .67, 8.2)));
        std::vector<urban::Point_2> vertices;
        vertices.push_back(urban::Point_2(A.x(), A.y()));
        vertices.push_back(urban::Point_2(B.x(), B.y()));
        vertices.push_back(urban::Point_2(C.x(), C.y()));
        
        std::vector<urban::Point_2> _vertices;
        _vertices.push_back(urban::Point_2(_A.x(), _A.y()));
        _vertices.push_back(urban::Point_2(_B.x(), _B.y()));
        _vertices.push_back(urban::Point_2(_C.x(), _C.y()));

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

        WHEN("Occlusion is computed")
        {
            std::list<urban::projection::FacePrint> rhs{{face_2}};
            std::list<urban::projection::FacePrint> result(urban::occlusion(face_1, rhs));
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << rhs.front() << result.front();
                REQUIRE(auxilary.str() == "The Polygon describing borders :3 -0.5 0.33 0.5 0.33 0 0.67  0 \nThe supporting plane coefficients : 0 -3.2 0.34 -0.644\nThe Polygon describing borders :3 -1 0 1 0 0 1  1 3 0.5 0.33 -0.5 0.33 0 0.67  \nThe supporting plane coefficients : 3 -0.23 2 -7\n");
            }
        }
    }
}
