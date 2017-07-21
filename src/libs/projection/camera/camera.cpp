#include "camera.h"

#include "../../algorithms/utils/util_algorithms.h"

namespace urban
{
    namespace projection
    {
        Camera::Camera(void) : name("N/A"), calibration(Affine_transformation_2()), position(Vector_3()), orientation(Affine_transformation_3()) {}

        Camera::Camera(std::string const& _name, double focal_distance, double scale_factor_x, double scale_factor_y, const Vector_2 & principal_point, const Vector_3 & _position, const std::map<double, Vector_3> & _rotations)
        : name(_name),
        calibration(Affine_transformation_2(focal_distance * scale_factor_x, .0, to_double(principal_point.x()), .0, focal_distance * scale_factor_y, to_double(principal_point.y()))),
        position(_position),
        orientation(rotation_transform(_rotations)) {}

        Camera::Camera(std::string const& _name, double focal_distance, const Vector_2 & principal_point, const Vector_3 & _position, const std::map<double, Vector_3> & _rotations)
        : name(_name),
        calibration(Affine_transformation_2(focal_distance, .0, to_double(principal_point.x()), .0, focal_distance, to_double(principal_point.y()))),
        position(_position),
        orientation(rotation_transform(_rotations)) {}

        Camera::Camera(std::string const& _name, const Affine_transformation_2 & _calibration, const Vector_3 & _position, const std::map<double, Vector_3> & _rotations)
        : name(_name),
        calibration(_calibration),
        position(_position),
        orientation(rotation_transform(_rotations)) {}

        Camera::Camera(std::string const& _name, const Affine_transformation_2 & _calibration, const Vector_3 & _position, const Affine_transformation_3 & _orientation)
        : name(_name),
        calibration(_calibration),
        position(_position),
        orientation(_orientation) {}

        Camera::Camera(const Camera & other)
        : name(other.name),
        calibration(other.calibration),
        position(other.position),
        orientation(other.orientation) {}
        
        Camera::Camera(Camera && other)
        : name(std::move(other.name)),
        calibration(std::move(other.calibration)),
        position(std::move(other.position)),
        orientation(std::move(other.orientation)) {}

        Camera::~Camera(void) {}

        void Camera::swap(Camera & other)
        {
            using std::swap;
            swap(name, other.name);
            swap(calibration, other.calibration);
            swap(position, other.position);
            swap(orientation, other.orientation);        
        }

        Camera & Camera::operator=(const Camera & other) noexcept
        {
            name = other.name;
            calibration = other.calibration;
            position = other.position;
            orientation = other.orientation;
            return *this;
        }

        Camera & Camera::operator=(Camera && other) noexcept
        {
            name = std::move(other.name);
            calibration = std::move(other.calibration);
            position = std::move(other.position);
            orientation = std::move(other.orientation);
            return *this;
        }
    }

    void swap(projection::Camera & lhs, projection::Camera & rhs)
    {
        lhs.swap(rhs);
    }
}
