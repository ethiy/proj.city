#include "../UrbanObject/Triangle/triangle.h"

#include "catch.hpp"

SCENARIO("Triangle manipulation:")
{
    GIVEN( "Three indices:" )
    {
        size_t indexes[3] = {145,45,97};

        WHEN( "the triangle is created")
        {
            urban::Triangle facet(indexes[0], indexes[1], indexes[2]);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "145 45 97 " );
            }
        }
    }
}
