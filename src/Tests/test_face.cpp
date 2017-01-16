#include "../ShadowMesh/Face/face.h"

#include "catch.hpp"

# include <cstdlib>

SCENARIO("Face manipulation:")
{
    GIVEN("Three indices:" )
    {
        size_t indexes[3] = {145,45,97};

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

        WHEN("the triangle is transformed to Lib3dsFace")
        {
            urban::Face facet(indexes[0], indexes[1], indexes[2]);
            Lib3dsFace* face_3ds(facet.to_3ds());
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

        WHEN("the triangle is created")
        {
            urban::Face facet(indexes.size(), indexes);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << facet;
                REQUIRE( auxilary.str() == "4 145 45 97 85 " );
            }
        }

        WHEN("the triangle is copied")
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

        WHEN("the triangle is inverted")
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

        WHEN("the triangle is iterated throught")
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

        WHEN("the triangle is transformed to Lib3dsFace")
        {
            urban::Face facet(indexes.size(), indexes);
            Lib3dsFace* face_3ds = facet.to_3ds();
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
