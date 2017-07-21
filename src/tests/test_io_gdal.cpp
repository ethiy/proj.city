#include "../libs/io/io_gdal.h"
#include "../libs/algorithms/algorithms.h"
#include "../libs/scene/unode.h"

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
        urban::shadow::Mesh test_mesh(
            "test_mesh",
            std::map<std::size_t, urban::shadow::Point>
            {
                {0, urban::shadow::Point(-10, 6, 8)},
                {1, urban::shadow::Point(-18, -14, 5)},
                {2, urban::shadow::Point(-2, -14, 6)},
                {3, urban::shadow::Point(-10, -10, -15)},
                {4, urban::shadow::Point(-2, 10, 0)},
                {5, urban::shadow::Point(-18, 9, 1)}
            },
            std::map<std::size_t, urban::shadow::Face>
            {
                {0, urban::shadow::Face{{0, 1, 2}}},
                {1, urban::shadow::Face{{4, 3, 5}}},
                {2, urban::shadow::Face{{0, 2, 4}}},
                {3, urban::shadow::Face{{3, 4, 2}}},
                {4, urban::shadow::Face{{5, 1, 0}}},
                {5, urban::shadow::Face{{1, 5, 3}}},
                {6, urban::shadow::Face{{5, 0, 4}}},
                {7, urban::shadow::Face{{1, 3, 2}}}
            }
        );
        urban::projection::BrickPrint test_proj = urban::project(urban::scene::Brick(test_mesh, urban::shadow::Point()));

        std::ostringstream file_name;

        WHEN("the projection is written to a shapefile")
        {
            file_name << boost::uuids::random_generator()() << ".gml";
            urban::io::FileHandler<GDALDriver> handler(
                 urban::io::GdalFormat::gml,
                boost::filesystem::path(file_name.str()),
                std::map<std::string,bool>{{"write", true}, {"read", true}}
            );
            handler.write(test_proj);
            THEN("The output checks:")
            {
                urban::projection::BrickPrint read_proj = handler.read<urban::projection::BrickPrint>();
                REQUIRE(read_proj == test_proj);
            }
        }
        WHEN("the projection is rasterized and written to a GeoTIFF")
        {
            file_name << boost::uuids::random_generator()() << ".geotiff";
            urban::io::FileHandler<GDALDriver> handler(
                urban::io::GdalFormat::geotiff,
                boost::filesystem::path(file_name.str()),
                std::map<std::string,bool>{{"write", true}, {"read", true}}
            );
            urban::projection::RasterPrint rasta = urban::rasterize(test_proj, 1, urban::shadow::Point());
            handler.write(rasta);
            THEN("The output checks:")
            {
                urban::projection::RasterPrint read_proj = handler.read<urban::projection::RasterPrint>();
                REQUIRE(read_proj == rasta);
            }
        }
    }
}
