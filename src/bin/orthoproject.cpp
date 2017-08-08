#include <config.h>

#include <urban.h>

#include <docopt.h>
#include <arguments.h>

#include <CGAL/IO/Geomview_stream.h>

#include <boost/filesystem.hpp>

#include <tinyxml2.h>

#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>

static const char USAGE[]=
R"(orthoproject.

    Usage:
      orthoproject <scene> [--pixel_size=<size> --rasterize --buildings --graphs --labels]
      orthoproject (-h | --help)
      orthoproject --version
    Options:
      -h --help             Show this screen.
      --version             Show version.
      --pixel_size=<size>   Pixel size [default: 1].
      --rasterize           Rasterize projections.
      --buildings           Save projections per building.
      --graphs              Save the Facets dual graph for buildings.
      --labels              Save vector projections with error fields.
)";

int main(int argc, const char** argv)
{
    try
    {
        Arguments arguments(
            docopt::docopt(
                USAGE,
                { argv + 1, argv + argc },
                true,
                "orthoproject " + std::string(VERSION)
            )
        );
        std::cout << std::boolalpha << arguments << std::endl;

        boost::filesystem::path root(arguments.input_path.parent_path());

        std::cout << "Parsing scene tree... " << std::flush;
        urban::scene::Scene scene(
            urban::io::FileHandler<tinyxml2::XMLDocument>(
                boost::filesystem::path(root / (arguments.input_path.stem().string() + ".XML"))
            ).read(
                urban::io::FileHandler<Lib3dsFile>(
                    arguments.input_path,
                    std::map<std::string,bool>{{"read", true}}
                )
            )
        );
        std::cout << "Done." << std::flush << std::endl;

        if(arguments.graphs)
            urban::save_building_duals(root, scene);
        
        auto projections = urban::orthoproject(scene);

        std::cout << "Summing and saving scene projections... " << std::flush;

        auto scene_projection = std::accumulate(
            std::begin(projections),
            std::end(projections),
            urban::projection::FootPrint()
        );

        urban::io::FileHandler<GDALDriver>(
            urban::io::GdalFormat::gml,
            boost::filesystem::path(root / (arguments.input_path.stem().string() + ".gml")),
            std::map<std::string,bool>{{"write", true}}
        ).write(scene_projection);
        std::cout << "Done." << std::flush << std::endl;
        
        std::cout << "Saving vector projections... " << std::flush;
        if(arguments.buildings)
        {
            boost::filesystem::path vector_dir(root / "vectors");
            boost::filesystem::create_directory(vector_dir);
            for(auto const& projection : projections)
            {
                urban::io::FileHandler<GDALDriver>(
                    urban::io::GdalFormat::gml,
                    boost::filesystem::path(vector_dir / (projection.get_name() + ".gml")),
                    std::map<std::string,bool>{{"write", true}}
                ).write(projection);
            }
            if(arguments.labels)
            {
                boost::filesystem::path label_dir(root / "labels");
                boost::filesystem::create_directory(label_dir);
                for(auto const& projection : projections)
                {
                    urban::io::FileHandler<GDALDriver>(
                        urban::io::GdalFormat::shapefile,
                        boost::filesystem::path(label_dir / (projection.get_name() + ".shp")),
                        std::map<std::string,bool>{{"write", true}}
                    ).write(projection, true);
                }
            }
        }

        std::cout << "Done." << std::flush << std::endl;


        if(arguments.rasterize)
        {
            std::cout << "rasterizing projections... " << std::flush;
            urban::projection::RasterPrint global_rasta(scene_projection, arguments.pixel_size);
            std::vector<urban::projection::RasterPrint> raster_projections(projections.size());
            std::transform(
                std::begin(projections),
                std::end(projections),
                std::begin(raster_projections),
                [&arguments](urban::projection::FootPrint const& projection)
                {
                    return urban::projection::RasterPrint(projection, arguments.pixel_size);
                }
            );
            std::cout << "Done." << std::flush << std::endl;

            if(arguments.buildings)
            {
                std::cout << "Saving raster projections... " << std::flush;
                urban::io::FileHandler<GDALDriver>(
                    urban::io::GdalFormat::geotiff,
                    boost::filesystem::path(root / (arguments.input_path.stem().string() + ".tiff")),
                    std::map<std::string,bool>{{"write", true}}
                ).write(global_rasta);
                boost::filesystem::path raster_dir(root / "rasters");
                boost::filesystem::create_directory(raster_dir);
                for(auto const& rasta : raster_projections)
                {
                    urban::io::FileHandler<GDALDriver>(
                        urban::io::GdalFormat::geotiff,
                        boost::filesystem::path(raster_dir / (rasta.get_name() + ".tiff")),
                        std::map<std::string,bool>{{"write", true}}
                    ).write(rasta);
                }
                std::cout << "Done." << std::flush << std::endl;
            }
        }
    }
    catch(std::exception const& except)
    {
        std::cerr << except.what() << std::flush << std::endl;
    }
    return EXIT_SUCCESS;
}
