#include <io/io_vector.h>
#include <io/io_raster.h>
#include <scene/unode.h>

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>
#include <fstream>
#include <streambuf>

#include <catch.hpp>

SCENARIO("Input/Output from Shadow Mesh:")
{
    GIVEN("A test shadow Mesh")
    {
        std::vector<urban::Point_3> points{{
            urban::Point_3(-10, 6, 8),
            urban::Point_3(-18, -14, 5),
            urban::Point_3(-2, -13, 6),
            urban::Point_3(-10, -10, -15),
            urban::Point_3(-2, 10, 0),
            urban::Point_3(-18, 9, 1)
        }};
        std::vector< std::vector<std::size_t> > polygons{{
            std::vector<std::size_t>{{0, 1, 2}},
            std::vector<std::size_t>{{4, 3, 5}},
            std::vector<std::size_t>{{0, 2, 4}},
            std::vector<std::size_t>{{3, 4, 2}},
            std::vector<std::size_t>{{5, 1, 0}},
            std::vector<std::size_t>{{1, 5, 3}},
            std::vector<std::size_t>{{5, 0, 4}},
            std::vector<std::size_t>{{1, 3, 2}}
        }};

        urban::projection::FootPrint test_footprint(
            urban::scene::UNode(
                "test_mesh",
                urban::shadow::Point(),
                2154,
                points,
                polygons
            )
        );

        std::ostringstream file_name;

        WHEN("the projection is written to a shapefile")
        {
            file_name << boost::uuids::random_generator()() << ".gml";
            urban::io::VectorHandler handler(
                 urban::io::GdalFormat::gml,
                boost::filesystem::path(file_name.str()),
                std::map<std::string,bool>{{"write", true}, {"read", true}}
            );
            handler.write(test_footprint);
            THEN("The output checks:")
            {
                urban::projection::FootPrint read_proj = handler.read<urban::projection::FootPrint>();
                REQUIRE(read_proj.data() == test_footprint.data());
            }
        }
        WHEN("the projection is rasterized and written to a GeoTIFF")
        {
            file_name << boost::uuids::random_generator()() << ".geotiff";
            urban::io::VectorHandler handler(
                urban::io::GdalFormat::geotiff,
                boost::filesystem::path(file_name.str()),
                std::map<std::string,bool>{{"write", true}, {"read", true}}
            );
            urban::projection::RasterPrint rasta(test_footprint, 1);
            handler.write(rasta);
            THEN("The output checks:")
            {
                urban::projection::RasterPrint read_proj = handler.read<urban::projection::RasterPrint>();
                REQUIRE(read_proj == rasta);
            }
        }
    }
}
