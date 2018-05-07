#include <io/io_obj.h>
#include <io/io_off.h>
#include <io/Line/line.h>

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

SCENARIO("Input/Output from obj file:")
{
    GIVEN("A minimal obj file")
    {
        WHEN("reading in text mode")
        {
            auto meshes = city::io::WaveObjHandler(
                boost::filesystem::path("../../ressources/3dModels/OBJ/minimal.obj"),
                std::map<std::string, bool>{{"read", true}}
            ).read().data();

            THEN("the output checks")
            {
                std::ostringstream auxilary, out;
                std::copy(
                    std::begin(meshes),
                    std::end(meshes),
                    std::ostream_iterator<city::shadow::Mesh>(auxilary, "\n")
                );

                std::istringstream _auxilary(auxilary.str());
                std::list<std::string> lines;
                city::io::readlines(_auxilary, std::back_inserter(lines));
                std::copy(
                    std::begin(lines),
                    std::end(lines),
                    std::ostream_iterator<std::string>(out, "\n")
                );

                std::ifstream tmp("../../ressources/tests/obj_minimal.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE(out.str() == tmp_str);
            }
        }

        WHEN("the reading mode is not chosen")
        {
            city::io::WaveObjHandler handler(
                boost::filesystem::path("../../ressources/3dModels/OBJ/minimal.obj"),
                std::map<std::string, bool>{{}}
            );

            THEN("the reader throws")
            {
                REQUIRE_THROWS(handler.read().data());
            }
        }
    }

    GIVEN("A wrong file path")
    {
        WHEN("the reading mode is chosen")
        {
            city::io::WaveObjHandler handler(
                boost::filesystem::path("../ressources/3dModels/OBJ/minimal.obj"),
                std::map<std::string, bool>{{"read", true}}
            );

            THEN("the reader throws")
            {
                REQUIRE_THROWS(handler.read().data());
            }
        }
    }

    GIVEN("An existing set of objects")
    {
        auto meshes = city::io::WaveObjHandler(
            boost::filesystem::path("../../ressources/3dModels/OBJ/minimal.obj"),
            std::map<std::string, bool>{{"read", true}}
        ).read().data();

        WHEN("the writing mode is chosen")
        {
            std::ostringstream file_name;
            file_name << boost::uuids::random_generator()() << ".obj";

            city::io::WaveObjHandler handler(
                boost::filesystem::path(file_name.str()),
                meshes
            );
            handler.write();

            THEN("the input should check")
            {
                auto written_meshes = city::io::WaveObjHandler(
                    boost::filesystem::path(file_name.str()),
                    std::map<std::string, bool>{{"read", true}}
                ).read().data();

                std::ostringstream auxilary, out;
                std::copy(
                    std::begin(written_meshes),
                    std::end(written_meshes),
                    std::ostream_iterator<city::shadow::Mesh>(auxilary, "\n")
                );

                std::istringstream _auxilary(auxilary.str());
                std::vector<std::string> lines;
                city::io::readlines(_auxilary, std::back_inserter(lines));
                std::copy(std::begin(lines), std::end(lines), std::ostream_iterator<std::string>(out, "\n"));

                std::ifstream tmp("../../ressources/tests/obj_minimal.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE(out.str() == tmp_str);
            }
        }
        
        WHEN("the writing mode is not chosen")
        {
            city::io::WaveObjHandler handler("./hammerhead.off", std::map<std::string, bool>{{"write", false}});

            THEN("the writter throws")
            {
                REQUIRE_THROWS(handler.write());
            }
        }
    }
    GIVEN("two objects obj file")
    {

        WHEN("reading in text mode")
        {
            auto meshes = city::io::WaveObjHandler(
                boost::filesystem::path("../../ressources/3dModels/OBJ/two.obj"),
                std::map<std::string, bool>{{"read", true}}
            ).read().data();

            THEN("the output checks")
            {
                std::ostringstream auxilary, out;
                std::copy(
                    std::begin(meshes),
                    std::end(meshes),
                    std::ostream_iterator<city::shadow::Mesh>(auxilary, "\n")
                );

                std::istringstream _auxilary(auxilary.str());
                std::list<std::string> lines;
                city::io::readlines(_auxilary, std::back_inserter(lines));
                std::copy(
                    std::begin(lines),
                    std::end(lines),
                    std::ostream_iterator<std::string>(out, "\n")
                );

                std::ifstream tmp("../../ressources/tests/obj_two.txt");
                std::string tmp_str((std::istreambuf_iterator<char>(tmp)), std::istreambuf_iterator<char>());
                REQUIRE(out.str() == tmp_str);
            }
        }
    }
}
