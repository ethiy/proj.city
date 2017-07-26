#include "scene_projection.h"
#include "utils.h"

#include <algorithm>
#include <numeric>

namespace urban
{
    namespace projection
    {
        FootPrint::FootPrint(void)
        {}
        FootPrint::FootPrint(scene::UNode const& unode)
            : name(unode.get_name()), reference_point(unode.get_reference_point()), epsg_index(unode.get_epsg_index())
        {
            std::vector<FacePrint> prints = orthoprint(unode);

            projection = std::accumulate(
                std::begin(prints),
                std::end(prints),
                projection,
                [](BrickPrint & proj, FacePrint const& face_print)
                {
                    return proj + BrickPrint(face_print);
                }
            );
        }
        FootPrint::FootPrint(std::string const& _name, OGRLayer* projection_layer)
            : name(_name)
        {
            projection_layer->ResetReading();

            OGRFeature* ogr_facet;
            while((ogr_facet = projection_layer->GetNextFeature()) != NULL)
            {
                FacePrint facet(ogr_facet, projection_layer->GetLayerDefn());
                projected_facets.push_back(facet);
                OGRFeature::DestroyFeature(ogr_facet);
                projected_surface.join(facet.get_polygon());
                bounding_box += facet.bbox();
            }
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
        unsigned short const FootPrint::get_epsg(void) const noexcept
        {
            return epsg_index;
        }
        Bbox_2 const& FootPrint::bbox(void) const noexcept
        {
            return projection.bbox();
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

        FootPrint & FootPrint::operator +=(FootPrint & other)
        {
            if(reference_point != other.reference || epsg_index != other.epsg_index)
                throw std::logic_error("Feature not supported");
            
            projection += other.projection;
            return *this;
        }

        void FootPrint::to_ogr(GDALDataset* file, bool labels) const
        {
            OGRSpatialReference spatial_reference_system;
            spatial_reference_system.importFromEPSG(
                epsg_index
            );
            OGRLayer* projection_layer = file->CreateLayer(name.c_str(), &spatial_reference_system, wkbPolygon, NULL);
            if(projection_layer == NULL)
                throw std::runtime_error("GDAL could not create a projection layer!");
            
            OGRFieldDefn* plane_coefficient_a = new OGRFieldDefn("coeff_a", OFTReal);
            projection_layer->CreateField(plane_coefficient_a);
            OGRFieldDefn* plane_coefficient_b = new OGRFieldDefn("coeff_b", OFTReal);
            projection_layer->CreateField(plane_coefficient_b);
            OGRFieldDefn* plane_coefficient_c = new OGRFieldDefn("coeff_c", OFTReal);
            projection_layer->CreateField(plane_coefficient_c);
            OGRFieldDefn* plane_coefficient_d = new OGRFieldDefn("coeff_d", OFTReal);
            projection_layer->CreateField(plane_coefficient_d);
            if(labels)
            {
                OGRFieldDefn* unqualified_errors = new OGRFieldDefn("Unq_Errors", OFTString);
                projection_layer->CreateField(unqualified_errors);
                OGRFieldDefn* building_errors = new OGRFieldDefn("Bul_Errors", OFTString);
                projection_layer->CreateField(building_errors);
                OGRFieldDefn* facets_errors = new OGRFieldDefn("Fac_Errors", OFTString);
                projection_layer->CreateField(facets_errors);
            }

            for(auto const& facet : projected_facets)
            {
                OGRFeature* ogr_facet = facet.to_ogr(projection_layer->GetLayerDefn(), reference_point, labels);
                if(projection_layer->CreateFeature(ogr_facet) != OGRERR_NONE)
                    throw std::runtime_error("GDAL could not insert the facet in vector image!");
                OGRFeature::DestroyFeature(ogr_facet);
            }
        }

        std::ostream & operator <<(std::ostream & os, FootPrint const& footprint)
        {
            os << "Name: " << brick_projection.name << std::endl
               << "Reference Point: " << brick_projection.reference_point << std::endl
               << "EPSG index: " << brick_projection.epsg_index << std::endl
               << projection;
            
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

        FootPrint & operator +(FootPrint & lhs, FootPrint & rhs)
        {
            return lhs += rhs;
        }

        void FootPrint::swap(FootPrint & lhs, FootPrint & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
