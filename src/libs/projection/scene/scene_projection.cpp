#include "scene_projection.h"
#include "utils.h"

#include <algorithm>
#include <numeric>

namespace urban
{
    namespace projection
    {
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

        void FootPrint::swap(FootPrint & lhs, FootPrint & rhs)
        {
            lhs.swap(rhs);
        }
    }
}
