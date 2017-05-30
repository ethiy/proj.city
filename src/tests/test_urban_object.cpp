#include "../libs/scene/brick/brick.h"
#include "../libs/io/io_off.h"

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>
#include <streambuf>

#include <catch.hpp>

SCENARIO("Urban Brick manipulation:")
{
    GIVEN("A urban::shadow::Mesh object")
    {
        urban::shadow::Mesh mesh = urban::io::FileHandler<std::fstream>(
            boost::filesystem::path("../../ressources/3dModels/OFF/hammerhead.off"),
            std::map<std::string,bool>{{"read", true}}
        ).read();

        WHEN("the urban Brick is constructed")
        {
            urban::scene::Brick hammerhead(mesh, urban::shadow::Point());

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
