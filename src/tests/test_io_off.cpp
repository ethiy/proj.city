#include "../libs/io/io_off.h"
#include "../libs/io/Line/line.h"

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp> 

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <streambuf>

#include <catch.hpp>

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
                std::ostringstream auxilary, out;
                auxilary << mesh;

                std::istringstream _auxilary(auxilary.str());
                std::vector<std::string> lines;
                urban::io::readlines(_auxilary, std::back_inserter(lines));
                std::copy(std::next(std::begin(lines), 1), std::end(lines), std::ostream_iterator<std::string>(out, "\n"));

                std::ifstream tmp("../../ressources/tests/hammerhead_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( out.str() == tmp_str);
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
            urban::io::FileHandler<std::fstream> handler(boost::filesystem::path(file_name.str()), modes);
            handler.write(mesh);

            THEN("the input should check")
            {
                urban::io::FileHandler<std::fstream> checker_handler(boost::filesystem::path(file_name.str()), _modes);
                urban::shadow::Mesh written_mesh = checker_handler.read();

                std::ostringstream auxilary, out;
                auxilary << written_mesh;

                std::istringstream _auxilary(auxilary.str());
                std::vector<std::string> lines;
                urban::io::readlines(_auxilary, std::back_inserter(lines));
                std::copy(std::next(std::begin(lines), 1), std::end(lines), std::ostream_iterator<std::string>(out, "\n"));

                std::ifstream tmp("../../ressources/tests/hammerhead_shadow_mesh.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE( out.str() == tmp_str);
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
