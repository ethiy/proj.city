#include "../UrbanObject/Face/face.h"

#include "catch.hpp"

SCENARIO("Face manipulation:")
{
    GIVEN( "Three indices:" )
    {
        size_t indexes[3] = {145,45,97};

        WHEN( "the triangle is created")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "145 45 97 " );
            }
        }

        WHEN( "the triangle is copied")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            urban::Face facet2 = facet;

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "145 45 97 " );
            }
        }

        WHEN( "the triangle is inverted")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            facet.invert_orientation();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "145 97 45 " );
            }
        }

        WHEN( "the triangle is transformed to Lib3dsFace")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            Lib3dsFace* face_3ds = facet.to_3ds();
            std::ostringstream auxilary;
            auxilary << face_3ds->points[0] << " " << face_3ds->points[1] << " " << face_3ds->points[2];
            REQUIRE( auxilary.str() == "145 45 97" );
        }
    }
}
