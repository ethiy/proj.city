#include <algorithms/test_utils.h>

#include <scene/unode.h>
#include <projection/scene/scene_projection.h>

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
        urban::projection::BrickPrint lhs(
            test_facet_projection(
                to_exact(urban::InexactKernel::Point_3(-1., 0, 5.)),
                to_exact(urban::InexactKernel::Point_3(1., 0., 2.)),
                to_exact(urban::InexactKernel::Point_3(0, 1., 3.615))
            )
        );
        urban::projection::BrickPrint rhs(
            test_facet_projection(
                to_exact(urban::InexactKernel::Point_3(-.5, .33, 5.)),
                to_exact(urban::InexactKernel::Point_3(.5, .33, 5.)),
                to_exact(urban::InexactKernel::Point_3(0, .67, 8.2))
            )
        );

        WHEN("Occlusion is computed")
        {
            lhs.occlusion(rhs);

            THEN("the output checks:")
            {
                std::ostringstream auxilary, _auxilary;
                auxilary << *std::begin(lhs) << *std::begin(rhs);
                _auxilary << "The Polygon describing borders :3 -1 0 1 0 0 1  1 3 0.5 0.33 -0.5 0.33 0 0.67  " << std::endl
                          << "The supporting plane coefficients : 3 -0.23 2 -7" << std::endl
                          << "The Polygon describing borders :3 -0.5 0.33 0.5 0.33 0 0.67  0 " << std::endl
                          << "The supporting plane coefficients : 0 -3.2 0.34 -0.644" << std::endl;
                REQUIRE(auxilary.str() == _auxilary.str());
            }
        }
    }

    GIVEN("two non convex faces with holes")
    {
        
        urban::projection::BrickPrint lhs( 
            test_facet_projection(
                std::vector<urban::Point_2>{{
                    urban::Point_2(-10, 6),
                    urban::Point_2(-12, 0),
                    urban::Point_2(0, 0),
                    urban::Point_2(-6, -12),
                    urban::Point_2(2, -12),
                    urban::Point_2(2, 8),
                    urban::Point_2(-2, 4)
                }},
                urban::Plane_3(
                    urban::Point_3(-10, 6, 0),
                    urban::Point_3(-12, 0, 0),
                    urban::Point_3(0, 0, 0)                
                )
            )
        );

        urban::projection::BrickPrint rhs(
            test_facet_projection(
                std::vector<urban::Point_2>{{
                            urban::Point_2(-6, 8),
                            urban::Point_2(-10, -10),
                            urban::Point_2(2, -10),
                            urban::Point_2(-2, -2),
                            urban::Point_2(-4, 6),
                            urban::Point_2(4, 4)
                }},
                std::vector<urban::Point_2>{{
                    urban::Point_2(-6, 4),
                    urban::Point_2(-4, 2),
                    urban::Point_2(-6, 2)
                }},
                urban::Plane_3(
                    urban::Point_3(-6, 8, 10),
                    urban::Point_3(-10, 10, 10),
                    urban::Point_3(2, -10, 10)
                )
            )
        );
        WHEN("Occlusion is computed")
        {
            lhs.occlusion(rhs);

            THEN("the output checks:")
            {
                urban::projection::BrickPrint _rhs(
                    test_facet_projection(
                        std::vector<urban::Point_2>{{
                            urban::Point_2(-10, -10),
                            urban::Point_2(2, -10),
                            urban::Point_2(-2, -2),
                            urban::Point_2(-4, 6),
                            urban::Point_2(4, 4),
                            urban::Point_2(-6, 8)
                        }},
                        std::vector<urban::Point_2>{{
                            urban::Point_2(-4, 2),
                            urban::Point_2(-6, 2),
                            urban::Point_2(-6, 4)
                        }},
                        urban::Plane_3(
                            urban::Point_3(-6, 8, 10),
                            urban::Point_3(-10, 10, 10),
                            urban::Point_3(2, -10, 10)                
                        )
                    )
                );

                auto _lhs = urban::projection::BrickPrint(
                        test_facet_projection(
                            std::vector<urban::Point_2>{{
                                urban::Point_2(-6, 2),
                                urban::Point_2(-4, 2),
                                urban::Point_2(-6, 4)
                            }},
                            urban::Plane_3(
                                urban::Point_3(-10, 6, 0),
                                urban::Point_3(-12, 0, 0),
                                urban::Point_3(0, 0, 0)                
                            )
                        )
                    )
                    +
                    urban::projection::BrickPrint(
                        test_facet_projection(
                            std::vector<urban::Point_2>{{
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
                            }},
                            urban::Plane_3(
                                urban::Point_3(-10, 6, 0),
                                urban::Point_3(-12, 0, 0),
                                urban::Point_3(0, 0, 0)                
                            )
                        )
                    )
                    +
                    urban::projection::BrickPrint(
                        test_facet_projection(
                            std::vector<urban::Point_2>{{
                                urban::Point_2(2, 8),
                                urban::Point_2(-2/7, 40/7),
                                urban::Point_2(2, 4.8)
                            }},
                            std::vector<urban::Point_2>{{
                                urban::Point_2(-4, 2),
                                urban::Point_2(-6, 2),
                                urban::Point_2(-6, 4)
                            }},
                            urban::Plane_3(
                                urban::Point_3(-10, 6, 0),
                                urban::Point_3(-12, 0, 0),
                                urban::Point_3(0, 0, 0)                
                            )
                        )
                    )
                    +
                    urban::projection::BrickPrint(
                        test_facet_projection(
                            std::vector<urban::Point_2>{{
                                urban::Point_2(-6.63158, 5.15789),
                                urban::Point_2(-10, 6),
                                urban::Point_2(-12, 0),
                                urban::Point_2(-70/9, 0)
                            }},
                            urban::Plane_3(
                                urban::Point_3(-10, 6, 0),
                                urban::Point_3(-12, 0, 0),
                                urban::Point_3(0, 0, 0)                
                            )
                        )
                    );

                REQUIRE((lhs == _lhs && rhs == _rhs));
            }
        }
    }
    GIVEN("A closed shadow Mesh")
    {
        std::vector<urban::Point_3> points{{
            urban::Point_3(-10, 6, 8),
            urban::Point_3(-18, -14, 5),
            urban::Point_3(-2, -13, 6),
            urban::Point_3(-10, -10, -15),
            urban::Point_3(-2, 10, 0),
            urban::Point_3(-18, 9, 1)
        }};
        std::vector< std::vector<std::size_t> > polygons{{
            std::vector<std::size_t>{{0, 1, 2}},
            std::vector<std::size_t>{{4, 3, 5}},
            std::vector<std::size_t>{{0, 2, 4}},
            std::vector<std::size_t>{{3, 4, 2}},
            std::vector<std::size_t>{{5, 1, 0}},
            std::vector<std::size_t>{{1, 5, 3}},
            std::vector<std::size_t>{{5, 0, 4}},
            std::vector<std::size_t>{{1, 3, 2}}
        }};

        urban::projection::FootPrint test_footprint(
            urban::scene::UNode(
                "test_mesh",
                urban::shadow::Point(),
                2154,
                points,
                polygons
            )
        );
        THEN("The output checks:")
        {
            auto _test_brickprint = urban::projection::BrickPrint(
                    test_facet_projection(
                        std::vector<urban::Point_2>{{
                            urban::Point_2(-10, 6),
                            urban::Point_2(-18, -14),
                            urban::Point_2(-2, -13)
                        }},
                        urban::Plane_3(-17, -40, 312, -2426)
                    )
                )
                +
                urban::projection::BrickPrint(
                    test_facet_projection(
                        std::vector<urban::Point_2>{{
                            urban::Point_2(-18, 9),
                            urban::Point_2(-18, -14),
                            urban::Point_2(-10, 6)
                        }},
                        urban::Plane_3(-149, 32, 184, -3154)
                    )
                )
                +
                urban::projection::BrickPrint(
                    test_facet_projection(
                        std::vector<urban::Point_2>{{
                            urban::Point_2(-10, 6),
                            urban::Point_2(-2, -13),
                            urban::Point_2(-2, 10)
                        }},
                        urban::Plane_3(160, 48, 184, -160)
                    )
                )
                +
                urban::projection::BrickPrint(
                    test_facet_projection(
                        std::vector<urban::Point_2>{{
                            urban::Point_2(-18, 9),
                            urban::Point_2(-10, 6),
                            urban::Point_2(-2, 10)
                        }},
                        urban::Plane_3(-4, 120, 56, -1208)
                    )
                );

            REQUIRE(test_footprint.data() == _test_brickprint);
        }
    }
}
