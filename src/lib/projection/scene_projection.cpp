#include <projection/scene_projection.h>
#include <projection/raster_projection.h>

#include <projection/utilities.h>

#include <algorithm>
#include <numeric>

#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>


namespace city
{
    namespace projection
    {
        FootPrint::FootPrint(void)
        {}
        FootPrint::FootPrint(scene::UNode const& unode)
            : name(unode.get_name()), reference_point(unode.get_reference_point()), epsg_index(unode.get_epsg())
        {
            std::cout << "Projecting node: " << unode.get_name() << std::endl;
            std::vector<FacePrint> prints = orthoprint(unode);
            try
            {
                projection = tbb::parallel_reduce(
                    tbb::blocked_range<std::vector<FacePrint>::iterator>(
                        std::begin(prints),
                        std::end(prints)
                    ),
                    projection,
                    [](tbb::blocked_range<std::vector<FacePrint>::iterator> const& b_range, BrickPrint const& init)
                    {
                        return std::accumulate(
                            std::begin(b_range),
                            std::end(b_range),
                            init,
                            [](BrickPrint & proj, FacePrint const& face_print)
                            {
                                return proj + face_print;
                            }
                        );
                    },
                    std::plus<BrickPrint>()
                );
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        FootPrint::FootPrint(std::string const& _name, OGRLayer* projection_layer)
            : name(_name), projection(projection_layer)
        {
            auto epsg_buffer = projection_layer->GetSpatialRef()->GetEPSGGeogCS();
            epsg_buffer > 0 ? epsg_index =  static_cast<unsigned short>(epsg_buffer) : epsg_index = 2154;
        }
        FootPrint::FootPrint(FootPrint const& other)
            : name(other.name), reference_point(other.reference_point), epsg_index(other.epsg_index), projection(other.projection)
        {}
        FootPrint::FootPrint(FootPrint && other)
            : name(std::move(other.name)), reference_point(std::move(other.reference_point)), epsg_index(std::move(other.epsg_index)), projection(std::move(other.projection))
        {}
        FootPrint::~FootPrint(void)
        {}

        void FootPrint::swap(FootPrint & other)
        {
            using std::swap;

            swap(name, other.name);
            swap(reference_point, other.reference_point);
            swap(epsg_index, other.epsg_index);
            swap(projection, other.projection);
        }
        FootPrint & FootPrint::operator =(FootPrint const& other)
        {
            name = other.name;
            reference_point = other.reference_point;
            epsg_index = other.epsg_index;
            projection = other.projection;

            return *this;
        }
        FootPrint & FootPrint::operator =(FootPrint && other)
        {
            name = std::move(other.name);
            reference_point = std::move(other.reference_point);
            epsg_index = std::move(other.epsg_index);
            projection = std::move(other.projection);

            return *this;
        }

        std::string const& FootPrint::get_name(void) const noexcept
        {
            return name;
        }
        shadow::Point const& FootPrint::get_reference_point(void) const noexcept
        {
            return reference_point;
        }
        unsigned short FootPrint::get_epsg(void) const noexcept
        {
            return epsg_index;
        }
        Bbox_2 const& FootPrint::bbox(void) const noexcept
        {
            return projection.bbox();
        }
        BrickPrint const& FootPrint::data(void) const noexcept
        {
            return projection;
        }

        std::vector<double> FootPrint::areas(void) const
        {
            return projection.areas();
        }
        double FootPrint::area(void) const
        {
            return projection.area();
        }
        std::vector<double> FootPrint::edge_lengths(void) const
        {
            return projection.edge_lengths();
        }        
        double FootPrint::circumference(void) const
        {
            return projection.circumference();
        }

        FootPrint::iterator FootPrint::begin(void) noexcept
        {
            return projection.begin();
        }
        FootPrint::iterator FootPrint::end(void) noexcept
        {
            return projection.end();
        }
        FootPrint::const_iterator FootPrint::begin(void) const noexcept
        {
            return projection.begin();
        }
        FootPrint::const_iterator FootPrint::end(void) const noexcept
        {
            return projection.end();
        }
        FootPrint::const_iterator FootPrint::cbegin(void) const noexcept
        {
            return projection.cbegin();
        }
        FootPrint::const_iterator FootPrint::cend(void) const noexcept
        {
            return projection.cend();
        }

        FootPrint & FootPrint::operator +=(FootPrint const& other)
        {
            if(projection.empty())
                *this = other;
            else
            {
                if(reference_point != other.reference_point || epsg_index != other.epsg_index)
                    throw std::logic_error("Feature not supported");
                
                projection += other.projection;
            }
            return *this;
        }

        void FootPrint::to_ogr(GDALDataset* file, bool labels) const
        {
            OGRSpatialReference spatial_reference_system;
            spatial_reference_system.importFromEPSG(epsg_index);

            OGRLayer* projection_layer = file->CreateLayer(name.c_str(), &spatial_reference_system, wkbPolygon, nullptr);

            if(projection_layer == nullptr)
                throw std::runtime_error("GDAL could not create a projection layer!");
            projection.to_ogr(projection_layer, reference_point, labels);            
        }
        
        std::vector<double> FootPrint::rasterize(std::vector<double> const& image, shadow::Point const& top_left, std::size_t const height, std::size_t const width, double const pixel_size) const
        {
            return tbb::parallel_reduce(
                tbb::blocked_range<BrickPrint::const_iterator>(
                    std::begin(projection),
                    std::end(projection)
                ),
                image,
                [top_left, height, width, pixel_size](tbb::blocked_range<BrickPrint::const_iterator> const& b_range, std::vector<double> const& init)
                {
                    return std::accumulate(
                        std::begin(b_range),
                        std::end(b_range),
                        init,
                        [top_left, height, width, pixel_size](std::vector<double> const& _image, projection::FacePrint const& face_projection)
                        {
                            return face_projection.rasterize(_image, top_left, height, width, pixel_size);
                        }
                    );
                },
                [](std::vector<double> const& lhs, std::vector<double> const& rhs)
                {
                    std::vector<double> result(lhs.size());
                    std::transform(
                        std::begin(lhs),
                        std::end(lhs),
                        std::begin(rhs),
                        std::begin(result),
                        std::plus<double>()
                    );
                    return result;
                }
            );
        }

        std::ostream & operator <<(std::ostream & os, FootPrint const& footprint)
        {
            os << "Name: " << footprint.name << std::endl
               << "Reference Point: " << footprint.reference_point << std::endl
               << "EPSG index: " << footprint.epsg_index << std::endl
               << footprint.projection;
            
            return os;
        }
        bool operator ==(FootPrint const& lhs, FootPrint const& rhs)
        {
            return  lhs.reference_point == rhs.reference_point
                    &&
                    lhs.epsg_index == rhs.epsg_index
                    &&
                    lhs.projection == rhs.projection;
        }
        bool operator !=(FootPrint const& lhs, FootPrint const& rhs)
        {
            return !(lhs == rhs);
        }

        FootPrint operator +(FootPrint const& lhs, FootPrint const& rhs)
        {
            FootPrint result(lhs);
            return result += rhs;
        }


        ScenePrint::ScenePrint(void)
        {}
        ScenePrint::ScenePrint(scene::Scene const& scene)
            : pivot(scene.get_pivot()), epsg_index(scene.get_epsg()), buildings(scene.orthoproject(false)), terrain(FootPrint(scene.get_terrain()))
        {}
        ScenePrint::ScenePrint(ScenePrint const& other)
            : pivot(other.pivot), epsg_index(other.epsg_index), buildings(other.buildings), terrain(other.terrain)
        {}
        ScenePrint::ScenePrint(ScenePrint && other)
            : pivot(std::move(other.pivot)), epsg_index(std::move(other.epsg_index)), buildings(std::move(other.buildings)), terrain(std::move(other.terrain))
        {}
        ScenePrint::~ScenePrint(void)
        {}

        void ScenePrint::swap(ScenePrint & other)
        {
            using std::swap;

            swap(pivot, other.pivot);
            swap(epsg_index, other.epsg_index);
            swap(buildings, other.buildings);
            swap(terrain, other.terrain);
        }
        ScenePrint& ScenePrint::operator =(ScenePrint const& other)
        {
            pivot = other.pivot;
            epsg_index = other.epsg_index;
            buildings = other.buildings;
            terrain = other.terrain;

            return *this;
        }
        ScenePrint& ScenePrint::operator =(ScenePrint && other)
        {
            pivot = std::move(other.pivot);
            epsg_index = std::move(other.epsg_index);
            buildings = std::move(other.buildings);
            terrain = std::move(other.terrain);

            return *this;
        }

        Bbox_2 ScenePrint::bbox(void) const
        {
            return std::accumulate(
                std::begin(buildings),
                std::end(buildings),
                terrain.bbox(),
                [](Bbox_2 const& bb, FootPrint const& nodeprint)
                {
                    return bb + nodeprint.bbox();
                }
            );
        }

        std::vector<double> ScenePrint::areas(void) const
        {
            std::vector<double> _areas(buildings.size());
            std::transform(
                std::begin(buildings),
                std::end(buildings),
                std::begin(_areas),
                [](FootPrint const& nodeprint)
                {
                    return nodeprint.area();
                }
            );
            return _areas;
        }
        std::vector<double> ScenePrint::circumferences(void) const
        {
            std::vector<double> _circumferences(buildings.size());
            std::transform(
                std::begin(buildings),
                std::end(buildings),
                std::begin(_circumferences),
                [](FootPrint const& nodeprint)
                {
                    return nodeprint.circumference();
                }
            );
            return _circumferences;
        }

        std::vector<RasterPrint> ScenePrint::rasterize(double const pixel_size) const
        {
            std::cout << "rasterizing projections... " << std::flush;
            std::vector<projection::RasterPrint> raster_projections(buildings.size());
            std::transform(
                std::begin(buildings),
                std::end(buildings),
                std::begin(raster_projections),
                [pixel_size, this](projection::FootPrint const& projection)
                {
                    return projection::RasterPrint(projection, pixel_size, terrain);
                }
            );
            std::cout << "Done." << std::flush << std::endl;
            return raster_projections;
        }
    }

    void swap(projection::FootPrint & lhs, projection::FootPrint & rhs)
    {
        lhs.swap(rhs);
    }

    std::vector<double> areas(projection::FootPrint const& footprint)
    {
        return footprint.areas();
    }
    double area(projection::FootPrint const& footprint)
    {
        return footprint.area();
    }
    std::vector<double> edge_lengths(projection::FootPrint const& footprint)
    {
        return footprint.edge_lengths();
    }
    double circumference(projection::FootPrint const& footprint)
    {
        return footprint.circumference();
    }
}
