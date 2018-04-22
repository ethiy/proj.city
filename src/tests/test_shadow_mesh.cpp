#include <shadow/mesh.h>
#include <io/io_off.h>

#include <boost/filesystem.hpp>

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

        urban::shadow::Mesh _u_mesh(
            "",
            std::vector<urban::shadow::Point>{{
                urban::shadow::Point(15.5343f, -13.4504f, 60.8789f),
                urban::shadow::Point(15.7204f, -13.188f, 60.8789f),
                urban::shadow::Point(15.7204f, -13.188f, 61.1764f)
            }},
            std::vector<urban::shadow::Face>{{urban::shadow::Face{{0, 2, 1}}}}
        );

        WHEN( "the mesh is created:")
        {
            urban::shadow::Mesh u_mesh(test_mesh);
            THEN("the output checks:")
            {
                REQUIRE( u_mesh == _u_mesh );
            }
        }
        WHEN( "mesh points and faces are accessed:")
        {
            urban::shadow::Mesh u_mesh(test_mesh);
            std::vector<urban::shadow::Point> points = u_mesh.get_points();
            std::vector<urban::shadow::Face> faces = u_mesh.get_faces();
            THEN("the output checks:")
            {
                std::ostringstream auxilary, _auxilary;
                auxilary << u_mesh;
                _auxilary << "#Name: " << std::endl
                          << "#Bounding box: " << u_mesh.bbox() << std::endl
                          << "OFF" << std::endl
                          << points.size() << " " << faces.size() << " 0" << std::endl;
                
                for(std::size_t idx(0); idx < points.size(); ++idx)
                        _auxilary << points.at(idx).x() << " " << points.at(idx).y() << " " << points.at(idx).z() << std::endl;

                for(std::size_t idx(0); idx < faces.size(); ++idx)
                        _auxilary << faces.at(idx) << std::endl;

                REQUIRE( auxilary.str() == _auxilary.str() );
            }
        }

        WHEN( "shadow::Mesh is converted back to lib3ds format and to \'urban::shadow::Mesh\' again: ")
        {
            urban::shadow::Mesh u_mesh(test_mesh);
            THEN("the output checks:")
            {
                Lib3dsMesh* _mesh = u_mesh.to_3ds();
                urban::shadow::Mesh _u_mesh(_mesh);
                lib3ds_mesh_free(_mesh);
                REQUIRE( u_mesh == _u_mesh );
            }
        }
        lib3ds_mesh_free(test_mesh);
    }

    GIVEN( "some entry meshes:")
    {
        WHEN("they are stitched")
        {
             auto mesh = urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29051.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29054.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29057.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29060.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29063.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29066.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/F29069.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/T11107.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read()
            +
            urban::io::OFFHandler(
                boost::filesystem::path("../../ressources/3dModels/OFF/T11108.off"),
                std::map<std::string, bool>{{"read", true}}
            ).read();
 
            THEN("The output checks:")
            {
                auto test_mesh = urban::io::OFFHandler(
                    boost::filesystem::path("../../ressources/tests/building_sum_mesh.off"),
                    std::map<std::string, bool>{{"read", true}}
                ).read();
                
                REQUIRE(mesh == test_mesh);
            }
        }
    }
}
