#include "../IO/io_off.h"

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>
#include <streambuf>

#include "catch.hpp"

SCENARIO("Input/Output from OFF file:")
{
    GIVEN("An existing OFF file")
    {
        boost::filesystem::path filepath("../../ressources/3dModels/OFF/hammerhead.off");
        
        WHEN("reading in text mode")
        {
            std::map<std::string,bool> modes{{"read", true}};
            urban::io::FileHandler<std::fstream> handler(filepath, modes);
            urban::ShadowMesh mesh = handler.read();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << mesh;

                std::ifstream tmp("../../ressources/tests/hammerhead_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
        
        WHEN("the reading mode is not chosen")
        {
            std::map<std::string,bool> modes;
            urban::io::FileHandler<std::fstream> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS( urban::ShadowMesh mesh = handler.read(), boost::filesystem::filesystem_error );
            }
        }
    }
    
    GIVEN("A wrong file path")
    {
        boost::filesystem::path filepath("../ressources/3dModels/OFF/hammerhead.off");
        
        WHEN("the reading mode is chosen")
        {
            std::map<std::string,bool> modes{{"read", true}};
            urban::io::FileHandler<std::fstream> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS( urban::ShadowMesh mesh = handler.read(), boost::filesystem::filesystem_error );
            }
        }
        
        WHEN("the reading mode is not chosen")
        {
            std::map<std::string,bool> modes;
            urban::io::FileHandler<std::fstream> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS( urban::ShadowMesh mesh = handler.read(), boost::filesystem::filesystem_error );
            }
        }
    }

    /*GIVEN("A A urban::ShadowMesh object")
    {
        boost::filesystem::path _filepath("../../ressources/3dModels/OFF/hammerhead.off");
        std::map<std::string,bool> _modes{{"read", true}};
        urban::io::FileHandler<std::fstream> extractor(_filepath, _modes);
        urban::ShadowMesh mesh = extractor.read();

        WHEN("the writing mode is chosen")
        {
            boost::filesystem::path filepath("test_output.off");
            std::map<std::string,bool> modes{{"write", true}};
            urban::io::FileHandler<std::fstream> writer(filepath, modes);
            writer.write(mesh);

            urban::io::FileHandler<std::fstream> checker(filepath, _modes);
            urban::ShadowMesh written_mesh = checker.read();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << written_mesh;

                std::ifstream tmp("../../ressources/tests/hammerhead_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
        
        WHEN("the writing mode is not chosen")
        {
            //Not specified!! The writer is not of interest for now
        }
    }*/
}
