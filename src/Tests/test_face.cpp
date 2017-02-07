#include "../geometry_definitions.h"

#include "../ShadowMesh/Face/face.h"

#include "catch.hpp"

# include <cstdlib>

SCENARIO("Face manipulation:")
{
    GIVEN("Two indexes")
    {
        std::vector<size_t> indexes{{145,45}};
        WHEN("the triangle is created")
        {
            THEN("The constructor throws")
            {
                REQUIRE_THROWS_AS( urban::Face facet(2, indexes), std::out_of_range );
            }
        }

    }

    GIVEN("Three indexes" )
    {
        size_t indexes[3] = {145,45,97};
        std::map<size_t, urban::Point> coord{{145, urban::Point(15.5343f, -13.4504f, 60.8789f)}, {45, urban::Point(15.7204f, -13.188f, 60.8789f)}, {97, urban::Point(15.7204f, -13.188f, 61.1764f)}};

        WHEN("the triangle is created")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "3 145 45 97 " );
            }
        }

        WHEN("the triangle is copied")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            urban::Face facet2 = facet;

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "3 145 45 97 " );
            }
        }

        WHEN("the triangle is inverted")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            facet.invert_orientation();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "3 145 97 45 " );
            }
        }

        WHEN("the triangle is iterated throught")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            std::ostringstream s_facet("");
            s_facet << facet.size() << " ";
            std::copy(std::begin(facet), std::end(facet), std::ostream_iterator<size_t>(s_facet));
            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "3 145 45 97 " );
            }
        }

        WHEN("Convexity is checked")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            THEN("the output checks")
            {
                REQUIRE( facet.is_convex(coord) );
            }
        }

        WHEN("the triangle is transformed to Lib3dsFace")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            Lib3dsFace* face_3ds(facet.to_3ds(coord));
            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << face_3ds->points[0] << " " << face_3ds->points[1] << " " << face_3ds->points[2];
                REQUIRE( auxilary.str() == "145 45 97" );
            }
            std::free(face_3ds);
        }
    }

    GIVEN("Four indices:" )
    {
        std::vector<size_t> indexes{{145,45,97,85}};
        WHEN("the facet is created")
        {
            urban::Face facet(indexes.size(), indexes);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "4 145 45 97 85 " );
            }
        }

        WHEN("the facet is copied")
        {
            urban::Face facet(indexes.size(), indexes);
            urban::Face facet2 = facet;

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "4 145 45 97 85 " );
            }
        }

        WHEN("the facet is inverted")
        {
            urban::Face facet(indexes.size(), indexes);
            facet.invert_orientation();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "4 145 85 97 45 " );
            }
        }

        WHEN("the facet is iterated throught")
        {
            urban::Face facet(indexes.size(), indexes);
            std::ostringstream s_facet("");
            s_facet << facet.size() << " ";
            std::copy(std::begin(facet), std::end(facet), std::ostream_iterator<size_t>(s_facet));
            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "4 145 45 97 85 " );
            }
        }
        
        WHEN("Convexity is checked with the wrong number of points")
        {
            urban::Face facet(indexes.size(), indexes);
            std::map<size_t, urban::Point> coord{{145, urban::Point(15.5343f, -13.4504f, 60.8789f)}, {45, urban::Point(15.7204f, -13.188f, 60.8789f)}, {97, urban::Point(15.7204f, -13.188f, 61.1764f)}};
            THEN("It throws")
            {
                REQUIRE_THROWS_AS( facet.is_convex(coord), std::out_of_range );
            }
        }

        WHEN("Convexity is checked for a non simple polygon")
        {
            urban::Face facet(indexes.size(), indexes);
            std::map<size_t, urban::Point> coord{{145, urban::Point(15.5343f, -13.4504f, 60.8789f)}, {85, urban::Point(15.7204f, -13.188f, 60.8789f)}, {97, urban::Point(15.65f, -12.988f, 60.8789f)}, {45, urban::Point(15.65f, -14.325f, 60.8789f)}};
            THEN("It does not checkout ")
            {
                REQUIRE( !facet.is_convex(coord));
            }
        }

        WHEN("Convexity is checked for a convex rectangle")
        {
            urban::Face facet(indexes.size(), indexes);
            std::map<size_t, urban::Point> coord{{145, urban::Point(15.5343f, -13.4504f, 60.8789f)}, {97, urban::Point(15.7204f, -13.188f, 60.8789f)}, {85, urban::Point(15.65f, -12.988f, 60.8789f)}, {45, urban::Point(15.65f, -14.325f, 60.8789f)}};
            THEN("It does checkout ")
            {
                REQUIRE( facet.is_convex(coord));
            }
        }

        WHEN("Convexity is checked for a non convex (simple) rectangle")
        {
            urban::Face facet(indexes.size(), indexes);
            std::map<size_t, urban::Point> coord{{145, urban::Point(15.5343f, -13.4504f, 60.8789f)}, {97, urban::Point(15.7204f, -13.188f, 60.8789f)}, {85, urban::Point(15.65f, -14.f, 60.8789f)}, {45, urban::Point(15.65f, -14.325f, 60.8789f)}};
            THEN("It does checkout ")
            {
                REQUIRE( !facet.is_convex(coord));
            }
        }

        WHEN("the facet is transformed to Lib3dsFace")
        {
            urban::Face facet(indexes.size(), indexes);
            std::map<size_t, urban::Point> coord{{145, urban::Point(15.5343f, -13.4504f, 60.8789f)}, {45, urban::Point(15.7204f, -13.188f, 60.8789f)}, {97, urban::Point(15.7204f, -13.188f, 61.1764f)}};
            Lib3dsFace* face_3ds = facet.to_3ds(coord);
            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << face_3ds->points[0] << " " << face_3ds->points[1] << " " << face_3ds->points[2] << " and " << (face_3ds + 1)->points[0] << " " << (face_3ds + 1)->points[1] << " " << (face_3ds + 1)->points[2];
                REQUIRE( auxilary.str() == "145 45 97 and 145 97 85" );
            }
            std::free(face_3ds);
        }
    }
}
