#include "../UrbanObject/Mesh/mesh.h"

#include "catch.hpp"

#include <initializer_list>
#include <iterator>
#include <algorithm>

SCENARIO("Mesh manipulation:")
{
    GIVEN( "A Lib3ds mesh:")
    {
        Lib3dsMesh* test_mesh = reinterpret_cast<Lib3dsMesh*>( calloc(sizeof(Lib3dsMesh),1) );
        test_mesh->faces = 1;
        auto init0 = std::initializer_list<Lib3dsWord>({0, 1, 2});
        //std::copy(std::begin(init0), std::end(init0), (test_mesh->faceL)->points);
        test_mesh->points = 3;
        auto init1 = std::initializer_list<float>({15.5343, -13.4504, 60.8789});
        std::copy(std::begin(init1), std::end(init1), (test_mesh->pointL)->pos);
        init1 = std::initializer_list<float>({15.7204, -13.188, 60.8789});
        std::copy(std::begin(init1), std::end(init1), (test_mesh->pointL + 1)->pos) ;
        init1 = std::initializer_list<float>({15.7204, -13.188, 61.1764});
        std::copy(std::begin(init1), std::end(init1), (test_mesh->pointL + 2)->pos);

        WHEN( "the mesh is created")
        {
            urban::Mesh u_mesh(*test_mesh);
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << u_mesh;
                REQUIRE( auxilary.str() == "Point 0 : 15.5343 -13.4504 60.8789\nPoint 1 : 15.7204 -13.188 60.8789\nPoint 2 : 15.5343 -13.4504 60.8789\nTriangle 0: 0 1 2 \n" );
            }
        }
    }
}
