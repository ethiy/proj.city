#include "../libs/io/io_3ds.h"

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 

#include <string>
#include <fstream>
#include <streambuf>

#include <catch.hpp>

SCENARIO("Input/Output from 3dsMAX file:")
{
    GIVEN("An existing 3dsMAX file")
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        
        WHEN("the reading mode is chosen")
        {
            std::vector<urban::shadow::Mesh> meshes = urban::io::FileHandler<Lib3dsFile>(filepath, std::map<std::string,bool>{{"read", true}}).read();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::shadow::Mesh>(auxilary, "\n"));

                std::ifstream tmp("../../ressources/tests/santa_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
        
        WHEN("the reading mode is not chosen")
        {
            urban::io::FileHandler<Lib3dsFile> handler(filepath, std::map<std::string,bool>{{"write", true}});

            THEN("the reader throws")
            {
                REQUIRE_THROWS( handler.read() );
            }
        }
    }
    
    GIVEN("A wrong file path")
    {
        boost::filesystem::path filepath("../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        
        WHEN("the reading mode is chosen")
        {
            THEN("the reader throws")
            {
                REQUIRE_THROWS( urban::io::FileHandler<Lib3dsFile> handler(filepath, std::map<std::string,bool>{{"read", true}}) );
            }
        }

        WHEN("No mode is chosen")
        {
            THEN("the reader throws")
            {
                REQUIRE_THROWS( urban::io::FileHandler<Lib3dsFile> handler(filepath, std::map<std::string,bool>{{}}) );
            }
        }
    }

    GIVEN("existing vector of urban::shadow::Mesh")
    {
        std::vector<urban::shadow::Mesh> meshes = urban::io::FileHandler<Lib3dsFile>(
            boost::filesystem::path("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS"),
            std::map<std::string,bool>{{"read", true}}
        ).read();

        WHEN("the writing mode is chosen")
        {
            std::ostringstream file_name;
            file_name << boost::uuids::random_generator()() << ".3ds";

            urban::io::FileHandler<Lib3dsFile>(
                boost::filesystem::path(file_name.str()),
                std::map<std::string,bool>{{"write", true}}
            ).write(meshes);

            THEN("the output checks")
            {
                std::vector<urban::shadow::Mesh> written_meshes = urban::io::FileHandler<Lib3dsFile>(
                    boost::filesystem::path(file_name.str()),
                    std::map<std::string,bool>{{"read", true}}
                ).read();

                std::ostringstream auxilary;
                std::copy(std::begin(written_meshes), std::end(written_meshes), std::ostream_iterator<urban::shadow::Mesh>(auxilary, "\n"));

                std::ifstream tmp("../../ressources/tests/santa_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
        
        WHEN("the writing mode is not chosen")
        {
            urban::io::FileHandler<Lib3dsFile> handler(
                boost::filesystem::path("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS"),
                std::map<std::string,bool>{{"read", true}}
            );

            THEN("the reader throws")
            {
                REQUIRE_THROWS(handler.write(meshes));
            }
        }
    }
}
