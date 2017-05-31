#include <shadow/Face/face.h>

#include <catch.hpp>

# include <cstdlib>

SCENARIO("Face manipulation:")
{
    GIVEN("A triangular facet and a coordinates map:" )
    {
        urban::shadow::Face facet{{145,45,97}};
        std::map<size_t, urban::shadow::Point> coord{{145, urban::shadow::Point(15.5343, -13.4504, 60.8789)}, {45, urban::shadow::Point(15.7204, -13.188, 60.8789)}, {97, urban::shadow::Point(15.7204, -13.188, 61.1764)}};

        WHEN("Out stream is checked")
        {
            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "3 145 45 97" );
            }
        }

        WHEN("the triangle is inverted")
        {
            facet.invert_orientation();

            THEN("the output checks")
            {
                urban::shadow::Face test_face{{145,97,45}};
                REQUIRE( facet == test_face );
            }
        }

        WHEN("Convexity is checked")
        {
            THEN("the output checks")
            {
                REQUIRE( facet.is_convex(coord) );
            }
        }

        WHEN("the triangle is transformed to Lib3dsFace")
        {
            Lib3dsFace* face_3ds = facet.to_3ds(coord);
            THEN("the output checks")
            {
                urban::shadow::Face test_face(face_3ds->points[0], face_3ds->points[1], face_3ds->points[2], true);
                REQUIRE( facet == test_face );
            }
            if(face_3ds)
                std::free(face_3ds);
        }
    }

    GIVEN("Four indices:" )
    {
        urban::shadow::Face facet{{145,45,97,85}};
        WHEN("the facet is created")
        {
            THEN("Out stream is checked")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "4 145 45 97 85" );
            }
        }

        WHEN("the facet is inverted")
        {
            facet.invert_orientation();
            THEN("the output checks")
            {
                urban::shadow::Face test_facet{{145,85,97,45}};
                REQUIRE( facet == test_facet );
            }
        }
        
        WHEN("Convexity is checked with the wrong number of points")
        {
            std::map<std::size_t, urban::shadow::Point> coord{{145, urban::shadow::Point(15.5343, -13.4504, 60.8789)}, {45, urban::shadow::Point(15.7204, -13.188, 60.8789)}, {97, urban::shadow::Point(15.7204, -13.188, 61.1764)}};
            THEN("It throws")
            {
                REQUIRE_THROWS_AS(facet.is_convex(coord), std::out_of_range );
            }
        }

        WHEN("Convexity is checked for a non simple polygon")
        {
            std::map<std::size_t, urban::shadow::Point> coord{{145, urban::shadow::Point(15.5343, -13.4504, 60.8789)}, {85, urban::shadow::Point(15.7204, -13.188, 60.8789)}, {97, urban::shadow::Point(15.65, -12.988, 60.8789)}, {45, urban::shadow::Point(15.65, -14.325, 60.8789)}};
            THEN("It does not checkout ")
            {
                REQUIRE(!facet.is_convex(coord));
            }
        }

        WHEN("Convexity is checked for a convex rectangle")
        {
            std::map<std::size_t, urban::shadow::Point> coord{{145, urban::shadow::Point(15.5343, -13.4504, 60.8789)}, {97, urban::shadow::Point(15.7204, -13.188, 60.8789)}, {85, urban::shadow::Point(15.65, -12.988, 60.8789)}, {45, urban::shadow::Point(15.65, -14.325, 60.8789)}};
            THEN("It does checkout ")
            {
                REQUIRE(facet.is_convex(coord));
            }
        }

        WHEN("Convexity is checked for a non convex (simple) rectangle")
        {
            std::map<std::size_t, urban::shadow::Point> coord{{145, urban::shadow::Point(15.5343, -13.4504, 60.8789)}, {97, urban::shadow::Point(15.7204, -13.188, 60.8789)}, {85, urban::shadow::Point(15.65, -14., 60.8789)}, {45, urban::shadow::Point(15.65, -14.325, 60.8789)}};
            THEN("It does checkout ")
            {
                REQUIRE( !facet.is_convex(coord));
            }
        }

        WHEN("A convex facet is transformed to Lib3dsFace")
        {
            std::map<std::size_t, urban::shadow::Point> coord{{145, urban::shadow::Point(15.5343, -13.4504, 60.8789)}, {97, urban::shadow::Point(15.7204, -13.188, 60.8789)}, {85, urban::shadow::Point(15.65, -12.988, 60.8789)}, {45, urban::shadow::Point(15.65, -14.325, 60.8789)}};
            Lib3dsFace* face_3ds = facet.to_3ds(coord);
            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << face_3ds->points[0] << " " << face_3ds->points[1] << " " << face_3ds->points[2] << " and " << (face_3ds + 1)->points[0] << " " << (face_3ds + 1)->points[1] << " " << (face_3ds + 1)->points[2];
                REQUIRE( auxilary.str() == "145 45 97 and 145 97 85" );
            }
            if(face_3ds)
                std::free(face_3ds);
        }
    }
}
