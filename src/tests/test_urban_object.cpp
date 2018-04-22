#include <scene/unode.h>
#include <io/io_3ds.h>

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>

#include <catch.hpp>

SCENARIO("Urban Node manipulation:")
{
    GIVEN("A 3ds file")
    {
        urban::io::T3DSHandler test_file(
            boost::filesystem::path("../../ressources/3dModels/3DS/Toy/Toy Santa Claus N180816.3DS"),
            std::map<std::string,bool>{{"read", true}}
        );

        WHEN("the \"buildings\" are read into unodes")
        {
            urban::scene::UNode staff("Staff", urban::shadow::Point(), 0, std::set<char>{'S'}, test_file);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << staff;

                std::ifstream tmp("../../ressources/tests/staff_unstitched.off");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
        WHEN("the \"buildings\" are read into unodes and stitched")
        {
            urban::scene::UNode staff("Staff", urban::shadow::Point(), 0, std::set<char>{'S'}, test_file);

            THEN("the output checks")
            {
                std::ostringstream auxilary;
                auxilary << staff.stitch_borders();

                std::ifstream tmp("../../ressources/tests/staff.off");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( auxilary.str() == tmp_str );
            }
        }
    }
}
