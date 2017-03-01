#include "../libs/Shadow/mesh.h"
#include "../libs/Algorithms/shadow_algorithms.h"

#include "../libs/urban.h"

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW


#include <catch.hpp>

#include <map>
#include <initializer_list>
#include <iterator>
#include <algorithm>

#include <fstream>

#include <cstdlib>

SCENARIO("shadow::Mesh manipulation:")
{
    GIVEN( "A Lib3ds mesh:")
    {
        Lib3dsMesh* test_mesh = reinterpret_cast<Lib3dsMesh*>(calloc(sizeof(Lib3dsMesh),1));
        test_mesh->faces = 1;
        auto init0 = std::initializer_list<Lib3dsWord>({0, 1, 2});
        test_mesh->faceL = reinterpret_cast<Lib3dsFace*>(calloc(sizeof(Lib3dsFace),3));
        std::copy(std::begin(init0), std::end(init0), (test_mesh->faceL)->points);
        test_mesh->points = 3;
        test_mesh->pointL = reinterpret_cast<Lib3dsPoint*>(calloc(sizeof(Lib3dsPoint),3));
        auto init1 = std::initializer_list<float>({15.5343f, -13.4504f, 60.8789f});
        std::copy(std::begin(init1), std::end(init1), (test_mesh->pointL)->pos);
        init1 = std::initializer_list<float>({15.7204f, -13.188f, 60.8789f});
        std::copy(std::begin(init1), std::end(init1), (test_mesh->pointL + 1)->pos) ;
        init1 = std::initializer_list<float>({15.7204f, -13.188f, 61.1764f});
        std::copy(std::begin(init1), std::end(init1), (test_mesh->pointL + 2)->pos);

        WHEN( "the mesh is created:")
        {
            urban::shadow::Mesh u_mesh(test_mesh);
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << u_mesh;
                REQUIRE( auxilary.str() == "Name: \nBounding box: 15.5343 15.7204 -13.4504 -13.188 60.8789 61.1764\nPoints: \nPoint 0 : 15.5343 -13.4504 60.8789\nPoint 1 : 15.7204 -13.188 60.8789\nPoint 2 : 15.7204 -13.188 61.1764\nFaces: \nFace 0 : 3 0 2 1 \n" );
            }
        }
        WHEN( "mesh points and faces are accessed:")
        {
            urban::shadow::Mesh u_mesh(test_mesh);
            std::map<size_t, urban::shadow::Point> points = u_mesh.get_points();
            std::map<size_t, urban::shadow::Face> faces = u_mesh.get_faces();
            THEN("the output checks:")
            {
                std::ostringstream auxilary, _auxilary;
                auxilary << u_mesh;
                _auxilary << "Name: " << std::endl
                          << "Bounding box: " << u_mesh.bbox() << std::endl
                          << "Points: " << std::endl;
                std::for_each(
                    std::begin(points),
                    std::end(points),
                    [&](std::pair<size_t, urban::shadow::Point> p)
                    {
                        _auxilary << "Point " << p.first << " : " << p.second.x() << " " << p.second.y() << " " << p.second.z() << std::endl;
                    }
                );
                _auxilary << "Faces: " << std::endl;
                std::for_each(
                    std::begin(faces),
                    std::end(faces),
                    [&](std::pair<size_t, urban::shadow::Face> t)
                    {
                        _auxilary << "Face " << t.first << " : " << t.second << std::endl;
                    }
                );
                REQUIRE( auxilary.str() == _auxilary.str() );
            }
        }

        WHEN( "shadow::Mesh is converted back to lib3ds format and to \'urban::shadow::Mesh\' again: ")
        {
            urban::shadow::Mesh u_mesh(test_mesh);
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                Lib3dsMesh* _mesh = u_mesh.to_3ds();
                urban::shadow::Mesh _u_mesh(_mesh);
                std::free(_mesh);
                auxilary << _u_mesh;
                REQUIRE( auxilary.str() == "Name: \nBounding box: 15.5343 15.7204 -13.4504 -13.188 60.8789 61.1764\nPoints: \nPoint 0 : 15.5343 -13.4504 60.8789\nPoint 1 : 15.7204 -13.188 60.8789\nPoint 2 : 15.7204 -13.188 61.1764\nFaces: \nFace 0 : 3 0 2 1 \n" );
            }
        }

        std::free(test_mesh);
    }

    GIVEN("Two stitchable shadow meshes:")
    {
        urban::shadow::Mesh mesh_1(
            "first",
            std::map<size_t, urban::shadow::Point>{{0, urban::shadow::Point(0., 0., 0.)}, {1, urban::shadow::Point(3.5, 1.325, 0.58)}, {3, urban::shadow::Point(6.28, -.2, -.5)}, {2, urban::shadow::Point(4.1, 2.368, 1.2589)}, {4, urban::shadow::Point(-.25, 2.12, .98)}},
            std::map<size_t, urban::shadow::Face>{{0, urban::shadow::Face(0, 1, 4)}, {1, urban::shadow::Face(1, 3, 2)}, {2, urban::shadow::Face(1, 2, 4)}}
        );

        urban::shadow::Mesh mesh_2(
            "second",
            std::map<size_t, urban::shadow::Point>{{0, urban::shadow::Point(-.549, -8.2, -10.54)}, {1, urban::shadow::Point(3.5, 1.325, 0.58)}, {2, urban::shadow::Point(1.54, -7.98, -5.97)}, {3, urban::shadow::Point(1.014, -6.32, -7.12)}, {4, urban::shadow::Point(6.28, -.2, -.5)}, {5, urban::shadow::Point(0., 0., 0.)}, {6, urban::shadow::Point(-5., -4.95, -9.23)}},
            std::map<size_t, urban::shadow::Face>{{0, urban::shadow::Face(0, 2, 1)}, {1, urban::shadow::Face(1, 2, 4)}, {2, urban::shadow::Face(4, 2, 3)}, {3, urban::shadow::Face(5, 0, 1)}, {4, urban::shadow::Face(5, 6, 0)}}
        );

        WHEN("they are stitched together")
        {
            std::vector<urban::shadow::Mesh> result(urban::stitch(std::vector<urban::shadow::Mesh>{{mesh_1, mesh_2}}));
            THEN("the output checks:")
            {
                std::ostringstream auxilary;
                auxilary << result.at(0);
                REQUIRE(auxilary.str() == "Name: first_second\nBounding box: -5 6.28 -8.2 2.368 -10.54 1.2589\nPoints: \nPoint 0 : 0 0 0\nPoint 1 : 3.5 1.325 0.58\nPoint 2 : 4.1 2.368 1.2589\nPoint 3 : 6.28 -0.2 -0.5\nPoint 4 : -0.25 2.12 0.98\nPoint 5 : -0.549 -8.2 -10.54\nPoint 6 : 1.54 -7.98 -5.97\nPoint 7 : 1.014 -6.32 -7.12\nPoint 8 : -5 -4.95 -9.23\nFaces: \nFace 0 : 3 0 1 4 \nFace 1 : 3 1 3 2 \nFace 2 : 3 1 2 4 \nFace 3 : 3 5 6 1 \nFace 4 : 3 1 6 3 \nFace 5 : 3 3 6 7 \nFace 6 : 3 0 5 1 \nFace 7 : 3 0 8 5 \n");
            }
        }        
    }
}
