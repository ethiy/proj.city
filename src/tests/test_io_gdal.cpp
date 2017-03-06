#include "../libs/IO/io_gdal.h"
#include "../libs/Algorithms/algorithms.h"
#include "../libs/UrbanObject/brick.h"

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
            std::map<size_t, urban::shadow::Point>
            {
                {0, urban::shadow::Point(-10, 6, 8)},
                {1, urban::shadow::Point(-18, -14, 5)},
                {2, urban::shadow::Point(-2, -13, 6)},
                {3, urban::shadow::Point(-10, -10, -15)},
                {4, urban::shadow::Point(-2, 10, 0)},
                {5, urban::shadow::Point(-18, 9, 1)}
            },
            std::map<size_t, urban::shadow::Face>
            {
                {0, urban::shadow::Face(0, 1, 2)},
                {1, urban::shadow::Face(4, 3, 5)},
                {2, urban::shadow::Face(0, 2, 4)},
                {3, urban::shadow::Face(3, 4, 2)},
                {4, urban::shadow::Face(5, 1, 0)},
                {5, urban::shadow::Face(1, 5, 3)},
                {6, urban::shadow::Face(5, 0, 4)},
                {7, urban::shadow::Face(1, 3, 2)}
            }
        );
        urban::projection::BrickPrint test_proj = urban::project(urban::Brick(test_mesh));
        WHEN("the projection is written to a shapefile")
        {
            boost::uuids::uuid unique_name = boost::uuids::random_generator()();
            std::ostringstream file_name;
            file_name << unique_name << ".geojson";

            std::map<std::string,bool> modes{{"write", true}, {"read", true}};
            urban::io::FileHandler<GDALDriver> handler("GeoJSON", boost::filesystem::path(file_name.str()), modes);
            handler.write(test_proj);
            THEN("The output checks:")
            {
                urban::projection::BrickPrint read_proj = handler.read_vectorial();
                // REQUIRE(read_proj == test_proj);
                std::pair<size_t, size_t> sizes;
                std::vector<double> double_pixels = urban::rasterize(test_proj, .05, sizes);
                unsigned long long dynamic = std::pow(2, sizeof(uint16_t) * 8) - 1;
                std::vector<uint16_t> pixels(double_pixels.size());
                std::vector<double>::iterator maximum_it = std::max_element(std::begin(double_pixels), std::end(double_pixels));
                std::transform(
                    std::begin(double_pixels),
                    std::end(double_pixels),
                    std::begin(pixels),
                    [dynamic, &maximum_it](const double value)
                    {
                        return static_cast<uint16_t>( dynamic * value / (*maximum_it));
                    }
                );
                std::cout << sizes.first << " " << sizes.second << std::endl;
                std::ostringstream rasta_name;
                rasta_name << unique_name << ".tiff";
                urban::io::FileHandler<GDALDriver> rasta("GTiff", boost::filesystem::path(rasta_name.str()), modes);
                rasta.write(pixels, sizes);
                // std::copy(std::begin(pixels), std::end(pixels), std::ostream_iterator<double>(std::cout, " "));
                // std::cout << std::endl;
            }
        }
    }
}