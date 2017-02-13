#include "../IO/io_3ds.h"

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 

#include <string>
#include <fstream>
#include <streambuf>

#include "catch.hpp"

SCENARIO("Input/Output from 3dsMAX file:")
{
    GIVEN("An existing 3dsMAX file")
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        
        WHEN("the reading mode is chosen")
        {
            std::map<std::string,bool> modes{{"read", true}};
            urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);
            std::vector<urban::shadow::Mesh> meshes = handler.read();

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
            std::map<std::string,bool> modes;
            urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS( std::vector<urban::shadow::Mesh> meshes = handler.read() );
            }
        }
    }
    
    GIVEN("A wrong file path")
    {
        boost::filesystem::path filepath("../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        
        WHEN("the reading mode is chosen")
        {
            std::map<std::string,bool> modes{{"read", true}};
            urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS( std::vector<urban::shadow::Mesh> meshes = handler.read() );
            }
        }
        
        WHEN("the reading mode is not chosen")
        {
            std::map<std::string,bool> modes;
            urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS( std::vector<urban::shadow::Mesh> meshes = handler.read() );
            }
        }
    }

    GIVEN("existing vector of urban::shadow::Mesh")
    {
        boost::filesystem::path filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string,bool> _modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> reader(filepath, _modes);
        std::vector<urban::shadow::Mesh> meshes = reader.read();

        WHEN("the writing mode is chosen")
        {
            boost::uuids::uuid unique_name = boost::uuids::random_generator()();
            std::ostringstream file_name;
            file_name << unique_name << ".3ds";


            std::map<std::string,bool> modes{{"write", true}};
            urban::io::FileHandler<Lib3dsFile> handler(file_name.str(), modes);
            handler.write(meshes);

            THEN("the output checks")
            {
                urban::io::FileHandler<Lib3dsFile> checker_handler(file_name.str(), _modes);
                std::vector<urban::shadow::Mesh> written_meshes = checker_handler.read();

                std::ostringstream auxilary;
                std::copy(std::begin(written_meshes), std::end(written_meshes), std::ostream_iterator<urban::shadow::Mesh>(auxilary, "\n"));

                std::ifstream tmp("../../ressources/tests/santa_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
        
        WHEN("the writing mode is not chosen")
        {
            std::map<std::string,bool> modes;
            urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS(handler.write(meshes), boost::filesystem::filesystem_error);
            }
        }
    }
}
