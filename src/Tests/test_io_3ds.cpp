#include "../IO/io_3ds.h"

#include <boost/filesystem.hpp>

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
            std::vector<urban::ShadowMesh> meshes = handler.read();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                std::copy(std::begin(meshes), std::end(meshes), std::ostream_iterator<urban::ShadowMesh>(auxilary, "\n"));

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
                REQUIRE_THROWS( std::vector<urban::ShadowMesh> meshes = handler.read() );
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
                REQUIRE_THROWS( std::vector<urban::ShadowMesh> meshes = handler.read() );
            }
        }
        
        WHEN("the reading mode is not chosen")
        {
            std::map<std::string,bool> modes;
            urban::io::FileHandler<Lib3dsFile> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS( std::vector<urban::ShadowMesh> meshes = handler.read() );
            }
        }
    }

    /*GIVEN("A urban::ShadowMesh object")
    {
        boost::filesystem::path _filepath("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS");
        std::map<std::string,bool> _modes{{"read", true}};
        urban::io::FileHandler<Lib3dsFile> reader(_filepath, _modes);
        std::vector<urban::ShadowMesh> meshes;
        int exit_code = reader.read(meshes);

        WHEN("the writing mode is chosen")
        {
            boost::filesystem::path filepath("../../ressources/tests/3DS/Toy Santa Claus N180816.3DS");
            std::map<std::string,bool> modes{{"write", true}};
            urban::io::FileHandler<Lib3dsFile> writer(filepath, modes);
            exit_code = writer.write(meshes);

            std::map<std::string,bool> __modes{{"read", true}};
            urban::io::FileHandler<Lib3dsFile> handler(_filepath, __modes);
            std::vector<urban::ShadowMesh> _meshes;
            exit_code = handler.read(_meshes);


            THEN("the output checks")
            {
                std::ostringstream auxilary;
                std::copy(std::begin(_meshes), std::end(_meshes), std::ostream_iterator<urban::ShadowMesh>(auxilary, "\n"));

                std::ifstream tmp("../../ressources/tests/santa_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                //REQUIRE( auxilary.str() == tmp_str );
            }
        }
        
        WHEN("the writing mode is not chosen")
        {
            //Not specified!! The writer is not of interest for now
        }
    }*/
}
