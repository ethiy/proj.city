#include "../libs/algorithms/algorithms.h"
#include "../libs/brick/brick.h"

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
        std::vector<urban::Point_2> buffer{{
            urban::Point_2(A.x(), A.y()),
            urban::Point_2(B.x(), B.y()),
            urban::Point_2(C.x(), C.y())
        }};
        
        std::vector<urban::Point_2> _buffer{{
            urban::Point_2(_A.x(), _A.y()),
            urban::Point_2(_B.x(), _B.y()),
            urban::Point_2(_C.x(), _C.y())
        }};

        urban::projection::FacePrint face_1(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(buffer),
                    std::end(buffer)
                )
            ),
            urban::Plane_3(A, B, C)
        );
        urban::projection::FacePrint face_2(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(_buffer),
                    std::end(_buffer)
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
                std::ostringstream auxilary, _auxilary;
                auxilary << rhs.front() << result.front();
                _auxilary << "The Polygon describing borders :3 -0.5 0.33 0.5 0.33 0 0.67  0 " << std::endl 
                          << "The supporting plane coefficients : 0 -3.2 0.34 -0.644" << std::endl 
                          << "The Polygon describing borders :3 -1 0 1 0 0 1  1 3 0.5 0.33 -0.5 0.33 0 0.67  " << std::endl 
                          << "The supporting plane coefficients : 3 -0.23 2 -7" << std::endl;
                REQUIRE(auxilary.str() == _auxilary.str());
            }
        }
    }

    GIVEN("two non convex faces with holes")
    {
        std::vector<urban::Point_2> buffer{{
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
                    std::begin(buffer),
                    std::end(buffer)
                )
            ),
            urban::Plane_3(
                urban::Point_3(-10, 6, 0),
                urban::Point_3(-12, 0, 0),
                urban::Point_3(0, 0, 0)                
            )
        );

        buffer.clear();
        buffer = std::vector<urban::Point_2>{{
            urban::Point_2(-6, 4),
            urban::Point_2(-4, 2),
            urban::Point_2(-6, 2)
        }};

        std::list<urban::Polygon> hole_list{{urban::Polygon(std::begin(buffer), std::end(buffer))}};

        buffer.clear();
        buffer = std::vector<urban::Point_2>{{
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
                    std::begin(buffer),
                    std::end(buffer)
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
        WHEN("Occlusion is computed")
        {
            std::list<urban::projection::FacePrint> rhs{{face_2}};
            std::list<urban::projection::FacePrint> result(urban::occlusion(face_1, rhs));
            THEN("the output checks:")
            {
                buffer.clear();
                buffer = std::vector<urban::Point_2>{{
                    urban::Point_2(-10, -10),
                    urban::Point_2(2, -10),
                    urban::Point_2(-2, -2),
                    urban::Point_2(-4, 6),
                    urban::Point_2(4, 4),
                    urban::Point_2(-6, 8)
                }};

                std::vector<urban::Point_2> _buffer{{
                    urban::Point_2(-4, 2),
                    urban::Point_2(-6, 2),
                    urban::Point_2(-6, 4)
                }};

                hole_list.clear();
                hole_list = std::list<urban::Polygon>{{urban::Polygon(std::begin(_buffer), std::end(_buffer))}};

                std::list<urban::projection::FacePrint> _rhs{{
                    urban::projection::FacePrint(
                        urban::Polygon_with_holes(
                            urban::Polygon(
                                std::begin(buffer),
                                std::end(buffer)
                            ),
                            std::begin(hole_list),
                            std::end(hole_list)
                        ),
                        urban::Plane_3(
                            urban::Point_3(-6, 8, 10),
                            urban::Point_3(-10, 10, 10),
                            urban::Point_3(2, -10, 10)                
                        )
                    )
                }};

                buffer.clear();
                buffer = std::vector<urban::Point_2>{{
                    urban::Point_2(-6, 2),
                    urban::Point_2(-4, 2),
                    urban::Point_2(-6, 4)
                }};

                std::list<urban::projection::FacePrint> _result{{
                    urban::projection::FacePrint(
                        urban::Polygon_with_holes(
                            urban::Polygon(
                                std::begin(buffer),
                                std::end(buffer)
                            )
                        ),
                        urban::Plane_3(
                            urban::Point_3(-10, 6, 0),
                            urban::Point_3(-12, 0, 0),
                            urban::Point_3(0, 0, 0)                
                        )
                    )
                }};

                buffer.clear();
                buffer = std::vector<urban::Point_2>{{
                    urban::Point_2(-0.8, 5.2),
                    urban::Point_2(-2, 4),
                    urban::Point_2(-3.6, 4.4),
                    urban::Point_2(-2.5, 0),
                    urban::Point_2(0, -1.5),
                    urban::Point_2(-3, 2),
                    urban::Point_2(-10, -5),
                    urban::Point_2(-10, -6),
                    urban::Point_2(-6, -12),
                    urban::Point_2(-12, 2),
                    urban::Point_2(2, 4.5)
                }};
                _result.push_back(
                    urban::projection::FacePrint(
                        urban::Polygon_with_holes(
                            urban::Polygon(
                                std::begin(buffer),
                                std::end(buffer)
                            )
                        ),
                        urban::Plane_3(
                            urban::Point_3(-10, 6, 0),
                            urban::Point_3(-12, 0, 0),
                            urban::Point_3(0, 0, 0)                
                        )
                    )
                );


                buffer.clear();
                buffer = std::vector<urban::Point_2>{{
                    urban::Point_2(2, 8),
                    urban::Point_2(-2/7, 40/7),
                    urban::Point_2(2, 4.8)
                }};
                _result.push_back(
                    urban::projection::FacePrint(
                        urban::Polygon_with_holes(
                            urban::Polygon(
                                std::begin(buffer),
                                std::end(buffer)
                            ),
                            std::begin(hole_list),
                            std::end(hole_list)
                        ),
                        urban::Plane_3(
                            urban::Point_3(-10, 6, 0),
                            urban::Point_3(-12, 0, 0),
                            urban::Point_3(0, 0, 0)                
                        )
                    )
                );
                buffer.clear();
                buffer = std::vector<urban::Point_2>{{   
                    urban::Point_2(-6.63158, 5.15789),
                    urban::Point_2(-10, 6),
                    urban::Point_2(-12, 0),
                    urban::Point_2(-70/9, 0)
                }};
                _result.push_back(
                    urban::projection::FacePrint(
                        urban::Polygon_with_holes(
                            urban::Polygon(
                                std::begin(buffer),
                                std::end(buffer)
                            )
                        ),
                        urban::Plane_3(
                            urban::Point_3(-10, 6, 0),
                            urban::Point_3(-12, 0, 0),
                            urban::Point_3(0, 0, 0)                
                        )
                    )
                );

                REQUIRE(rhs == _rhs && result == _result);
            }
        }
    }
    GIVEN("A closed shadow Mesh")
    {
        urban::shadow::Mesh test_mesh(
            "test_mesh",
            std::map<std::size_t, urban::shadow::Point>{
                {0, urban::shadow::Point(-10, 6, 8)},
                {1, urban::shadow::Point(-18, -14, 5)},
                {2, urban::shadow::Point(-2, -13, 6)},
                {3, urban::shadow::Point(-10, -10, -15)},
                {4, urban::shadow::Point(-2, 10, 0)},
                {5, urban::shadow::Point(-18, 9, 1)}
            },
            std::map<std::size_t, urban::shadow::Face>{
                {0, urban::shadow::Face{{0, 1, 2}}},
                {1, urban::shadow::Face{{4, 3, 5}}},
                {2, urban::shadow::Face{{0, 2, 4}}},
                {3, urban::shadow::Face{{3, 4, 2}}},
                {4, urban::shadow::Face{{5, 1, 0}}},
                {5, urban::shadow::Face{{1, 5, 3}}},
                {6, urban::shadow::Face{{5, 0, 4}}},
                {7, urban::shadow::Face{{1, 3, 2}}}
            }
        );

        urban::Brick test_brick(test_mesh, urban::shadow::Point());

        urban::projection::BrickPrint test_proj = urban::project(test_brick);
        THEN("The output checks:")
        {
            std::stringstream auxilary;
            auxilary << test_proj;
            REQUIRE(auxilary.str() == "Name: test_mesh_xy\nBounding box: -18 -14 -2 10\nReference Point: 0 0 0\nEPSG index: 2154\nFace Projections: 4\nThe Polygon describing borders :3 -10 6 -18 -14 -2 -13  0 \nThe supporting plane coefficients : -17 -40 312 -2426\n\nThe Polygon describing borders :3 -18 9 -18 -14 -10 6  0 \nThe supporting plane coefficients : -149 32 184 -3154\n\nThe Polygon describing borders :3 -10 6 -2 -13 -2 10  0 \nThe supporting plane coefficients : 160 48 184 -160\n\nThe Polygon describing borders :3 -18 9 -10 6 -2 10  0 \nThe supporting plane coefficients : -4 120 56 -1208\n\nProjected surface: \n4 -18 -14 -2 -13 -2 10 -18 9  0 \n");
        }
    }
}
