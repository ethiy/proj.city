#include <projection/scene_projection.h>

#include <projection/utilities.h>

#include <algorithm>
#include <numeric>

namespace urban
{
    namespace projection
    {
        FootPrint::FootPrint(void)
        {}
        FootPrint::FootPrint(scene::UNode const& unode)
            : name(unode.get_name()), reference_point(unode.get_reference_point()), epsg_index(unode.get_epsg())
        {
            std::vector<FacePrint> prints = orthoprint(unode);

            projection = std::accumulate(
                std::begin(prints),
                std::end(prints),
                projection,
                [](BrickPrint & proj, FacePrint const& face_print)
                {
                    return proj + face_print;
                }
            );
        }
        FootPrint::FootPrint(std::string const& _name, OGRLayer* projection_layer)
            : name(_name), projection(projection_layer)
        {}
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

        double FootPrint::area(void) const
        {
            return projection.area();
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
            if(projection.is_empty())
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

            OGRLayer* projection_layer = file->CreateLayer(name.c_str(), &spatial_reference_system, wkbPolygon, NULL);

            if(projection_layer == NULL)
                throw std::runtime_error("GDAL could not create a projection layer!");
            projection.to_ogr(projection_layer, reference_point, labels);            
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
            return  lhs.name == rhs.name
                    &&
                    lhs.reference_point == rhs.reference_point
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
    }

    void swap(projection::FootPrint & lhs, projection::FootPrint & rhs)
    {
        lhs.swap(rhs);
    }

    double area(projection::FootPrint const& footprint)
    {
        return footprint.area();
    }
    double circumference(projection::FootPrint const& footprint)
    {
        return footprint.circumference();
    }
}
