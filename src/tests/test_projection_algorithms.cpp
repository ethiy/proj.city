#include <algorithms/test_utils.h>

#include <scene/unode.h>
#include <projection/scene_projection.h>

#include <CGAL/Boolean_set_operations_2.h>

#include <catch.hpp>

#include <vector>
#include <iterator>
#include <list>

SCENARIO("Occlusion management")
{
    GIVEN("two triangular faces")
    {
        city::InexactToExact to_exact;
        city::projection::BrickPrint lhs(
            test_facet_projection(
                0,
                to_exact(city::InexactKernel::Point_3(-1., 0, 5.)),
                to_exact(city::InexactKernel::Point_3(1., 0., 2.)),
                to_exact(city::InexactKernel::Point_3(0, 1., 3.615))
            )
        );
        auto rhs = test_facet_projection(
            1,
            to_exact(city::InexactKernel::Point_3(-.5, .33, 5.)),
            to_exact(city::InexactKernel::Point_3(.5, .33, 5.)),
            to_exact(city::InexactKernel::Point_3(0, .67, 8.2))
        );

        WHEN("Occlusion is computed")
        {
            lhs.occlusion(rhs);

            THEN("the output checks:")
            {
                std::ostringstream auxilary, _auxilary;
                auxilary << *std::begin(lhs) << rhs;
                _auxilary << "Id: 0" << std::endl
                          << "The Polygon describing borders :3 -1 0 1 0 0 1  1 3 0.5 0.33 -0.5 0.33 0 0.67  " << std::endl
                          << "The supporting plane coefficients : 3 -0.23 2 -7" << std::endl
                          << "Id: 1" << std::endl
                          << "The Polygon describing borders :3 -0.5 0.33 0.5 0.33 0 0.67  0 " << std::endl
                          << "The supporting plane coefficients : 0 -3.2 0.34 -0.644" << std::endl;
                REQUIRE(auxilary.str() == _auxilary.str());
            }
        }
    }

    GIVEN("two non convex faces with holes")
    {
        city::projection::BrickPrint lhs( 
            test_facet_projection(
                0,
                std::vector<city::Point_2>{{
                    city::Point_2(-10, 6),
                    city::Point_2(-12, 0),
                    city::Point_2(0, 0),
                    city::Point_2(-6, -12),
                    city::Point_2(2, -12),
                    city::Point_2(2, 8),
                    city::Point_2(-2, 4)
                }},
                city::Plane_3(
                    city::Point_3(-10, 6, 0),
                    city::Point_3(-12, 0, 0),
                    city::Point_3(0, 0, 0)                
                )
            )
        );

        auto rhs = test_facet_projection(
            1,
            std::vector<city::Point_2>{{
                city::Point_2(-6, 8),
                city::Point_2(-10, -10),
                city::Point_2(2, -10),
                city::Point_2(-2, -2),
                city::Point_2(-4, 6),
                city::Point_2(4, 4)
            }},
            std::vector<city::Point_2>{{
                city::Point_2(-6, 4),
                city::Point_2(-4, 2),
                city::Point_2(-6, 2)
            }},
            city::Plane_3(
                city::Point_3(-6, 8, 10),
                city::Point_3(-10, 10, 10),
                city::Point_3(2, -10, 10)
            )
        );
        WHEN("Occlusion is computed")
        {
            lhs.occlusion(rhs);

            THEN("the output checks:")
            {
                city::projection::BrickPrint _rhs(
                    test_facet_projection(
                        2,
                        std::vector<city::Point_2>{{
                            city::Point_2(-10, -10),
                            city::Point_2(2, -10),
                            city::Point_2(-2, -2),
                            city::Point_2(-4, 6),
                            city::Point_2(4, 4),
                            city::Point_2(-6, 8)
                        }},
                        std::vector<city::Point_2>{{
                            city::Point_2(-4, 2),
                            city::Point_2(-6, 2),
                            city::Point_2(-6, 4)
                        }},
                        city::Plane_3(
                            city::Point_3(-6, 8, 10),
                            city::Point_3(-10, 10, 10),
                            city::Point_3(2, -10, 10)                
                        )
                    )
                );

                auto _lhs = city::projection::BrickPrint(
                        test_facet_projection(
                            3,
                            std::vector<city::Point_2>{{
                                city::Point_2(-6, 2),
                                city::Point_2(-4, 2),
                                city::Point_2(-6, 4)
                            }},
                            city::Plane_3(
                                city::Point_3(-10, 6, 0),
                                city::Point_3(-12, 0, 0),
                                city::Point_3(0, 0, 0)                
                            )
                        )
                    )
                    +
                    city::projection::BrickPrint(
                        test_facet_projection(
                            4,
                            std::vector<city::Point_2>{{
                                city::Point_2(-0.8, 5.2),
                                city::Point_2(-2, 4),
                                city::Point_2(-3.6, 4.4),
                                city::Point_2(-2.5, 0),
                                city::Point_2(0, -1.5),
                                city::Point_2(-3, 2),
                                city::Point_2(-10, -5),
                                city::Point_2(-10, -6),
                                city::Point_2(-6, -12),
                                city::Point_2(-12, 2),
                                city::Point_2(2, 4.5)
                            }},
                            city::Plane_3(
                                city::Point_3(-10, 6, 0),
                                city::Point_3(-12, 0, 0),
                                city::Point_3(0, 0, 0)                
                            )
                        )
                    )
                    +
                    city::projection::BrickPrint(
                        test_facet_projection(
                            5,
                            std::vector<city::Point_2>{{
                                city::Point_2(2, 8),
                                city::Point_2(-2/7, 40/7),
                                city::Point_2(2, 4.8)
                            }},
                            std::vector<city::Point_2>{{
                                city::Point_2(-4, 2),
                                city::Point_2(-6, 2),
                                city::Point_2(-6, 4)
                            }},
                            city::Plane_3(
                                city::Point_3(-10, 6, 0),
                                city::Point_3(-12, 0, 0),
                                city::Point_3(0, 0, 0)                
                            )
                        )
                    )
                    +
                    city::projection::BrickPrint(
                        test_facet_projection(
                            6,
                            std::vector<city::Point_2>{{
                                city::Point_2(-6.63158, 5.15789),
                                city::Point_2(-10, 6),
                                city::Point_2(-12, 0),
                                city::Point_2(-70/9, 0)
                            }},
                            city::Plane_3(
                                city::Point_3(-10, 6, 0),
                                city::Point_3(-12, 0, 0),
                                city::Point_3(0, 0, 0)                
                            )
                        )
                    );

                REQUIRE((lhs == _lhs && rhs == _rhs));
            }
        }
    }
    GIVEN("A closed shadow Mesh")
    {
        std::vector<city::Point_3> points{{
            city::Point_3(-10, 6, 8),
            city::Point_3(-18, -14, 5),
            city::Point_3(-2, -13, 6),
            city::Point_3(-10, -10, -15),
            city::Point_3(-2, 10, 0),
            city::Point_3(-18, 9, 1)
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

        city::projection::FootPrint test_footprint(
            city::scene::UNode(
                "test_mesh",
                points,
                polygons,
                city::Point_3(),
                2154
            )
        );
        THEN("The output checks:")
        {
            auto _test_brickprint = city::projection::BrickPrint(
                    test_facet_projection(
                        0,
                        std::vector<city::Point_2>{{
                            city::Point_2(-10, 6),
                            city::Point_2(-18, -14),
                            city::Point_2(-2, -13)
                        }},
                        city::Plane_3(-17, -40, 312, -2426)
                    )
                )
                +
                city::projection::BrickPrint(
                    test_facet_projection(
                        1,
                        std::vector<city::Point_2>{{
                            city::Point_2(-18, 9),
                            city::Point_2(-18, -14),
                            city::Point_2(-10, 6)
                        }},
                        city::Plane_3(-149, 32, 184, -3154)
                    )
                )
                +
                city::projection::BrickPrint(
                    test_facet_projection(
                        2,
                        std::vector<city::Point_2>{{
                            city::Point_2(-10, 6),
                            city::Point_2(-2, -13),
                            city::Point_2(-2, 10)
                        }},
                        city::Plane_3(160, 48, 184, -160)
                    )
                )
                +
                city::projection::BrickPrint(
                    test_facet_projection(
                        3,
                        std::vector<city::Point_2>{{
                            city::Point_2(-18, 9),
                            city::Point_2(-10, 6),
                            city::Point_2(-2, 10)
                        }},
                        city::Plane_3(-4, 120, 56, -1208)
                    )
                );

            REQUIRE(test_footprint.data() == _test_brickprint);
        }
    }
}
