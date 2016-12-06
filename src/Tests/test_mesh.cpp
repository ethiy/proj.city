#include "../UrbanObject/Mesh/mesh.h"

#include "catch.hpp"

SCENARIO("Mesh manipulation:")
{
    GIVEN( "A Lib3ds mesh:")
    {
        Lib3dsMesh* test_mesh = reinterpret_cast<Lib3dsMesh*>( calloc(sizeof(Lib3dsMesh),1) );
        test_mesh->faces = 1;
        (test_mesh->faceL)->points = {1, 2, 3};
        test_mesh->points = 3;
        (test_mesh->pointL)->pos = {15.5343, -13.4504, 60.8789};
        (test_mesh->pointL + 1)->pos = {15.7204, -13.188, 60.8789};
        (test_mesh->pointL + 2)->pos = {15.7204, -13.188, 61.1764};

        WHEN( "the mesh is created")
        {
            urban::Mesh u_mesh(*test_mesh);
            Then("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << u_mesh;
                REQUIRE( auxilary.str() == "Point 0 : 15.5343 -13.4504 60.8789\n
                                            Point 1 : 15.7204 -13.188 60.8789\n
                                            Point 2 : 15.5343 -13.4504 60.8789\n
                                            Traingle 0: 0 1 2 \n" );
            }
        }
    }
}
