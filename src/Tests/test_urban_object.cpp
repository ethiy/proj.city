#include "../UrbanObject/brick.h"
#include "../IO/io_off.h"

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>
#include <streambuf>

#include "catch.hpp"

SCENARIO("Urban Brick manipulation:")
{
    GIVEN("A urban::ShadowMesh object")
    {
        std::map<std::string,bool> modes{{"read", true}};
        boost::filesystem::path filepath("../../ressources/3dModels/OFF/hammerhead.off");
        urban::io::FileHandler<std::fstream> handler(filepath, modes);
        urban::ShadowMesh mesh = handler.read();

        WHEN("the urban Brick is constructed")
        {
            urban::Brick hammerhead(mesh);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << hammerhead;

                std::ifstream tmp("../../ressources/tests/hammerhead.off");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
    }
}
