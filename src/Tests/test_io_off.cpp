#include "../IO/io_off.h"

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 

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
            std::map<std::string, bool> modes{{"read", true}};
            urban::io::FileHandler<std::fstream> handler(filepath, modes);
            urban::shadow::Mesh mesh = handler.read();

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << mesh;

                std::ifstream tmp("../../ressources/tests/hammerhead_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE(auxilary.str() == tmp_str);
            }
        }

        WHEN("the reading mode is not chosen")
        {
            std::map<std::string, bool> modes;
            urban::io::FileHandler<std::fstream> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS(urban::shadow::Mesh mesh = handler.read(), boost::filesystem::filesystem_error);
            }
        }
    }

    GIVEN("A wrong file path")
    {
        boost::filesystem::path filepath("../ressources/3dModels/OFF/hammerhead.off");

        WHEN("the reading mode is chosen")
        {
            std::map<std::string, bool> modes{{"read", true}};
            urban::io::FileHandler<std::fstream> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS(urban::shadow::Mesh mesh = handler.read(), boost::filesystem::filesystem_error);
            }
        }

        WHEN("the reading mode is not chosen")
        {
            std::map<std::string, bool> modes;
            urban::io::FileHandler<std::fstream> handler(filepath, modes);

            THEN("the reader throws")
            {
                REQUIRE_THROWS_AS(urban::shadow::Mesh mesh = handler.read(), boost::filesystem::filesystem_error);
            }
        }
    }

    GIVEN("An existing a urban::shadow::Mesh")
    {
        boost::filesystem::path _filepath("../../ressources/3dModels/OFF/hammerhead.off");
        std::map<std::string, bool> _modes{{"read", true}};
        urban::io::FileHandler<std::fstream> _handler(_filepath, _modes);
        urban::shadow::Mesh mesh = _handler.read();

        WHEN("the writing mode is chosen")
        {
            boost::uuids::uuid unique_name = boost::uuids::random_generator()();
            std::ostringstream file_name;
            file_name << unique_name << ".off";

            std::map<std::string, bool> modes{{"write", true}};
            urban::io::FileHandler<std::fstream> handler(file_name.str(), modes);
            handler.write(mesh);

            THEN("the input should check")
            {
                urban::io::FileHandler<std::fstream> checker_handler(file_name.str(), _modes);
                urban::shadow::Mesh written_mesh = checker_handler.read();

                std::ostringstream auxilary;
                auxilary << written_mesh;

                std::ifstream tmp("../../ressources/tests/hammerhead_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE(auxilary.str() == tmp_str);
            }
        }
        
        WHEN("the writing mode is not chosen")
        {
            std::map<std::string, bool> modes{{"write", false}};
            urban::io::FileHandler<std::fstream> handler("./hammerhead.off", modes);

            THEN("the writter throws")
            {
                REQUIRE_THROWS_AS(handler.write(mesh), boost::filesystem::filesystem_error);
            }
        }
    }
}
