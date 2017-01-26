#pragma once

#include "camera.h"

#include "../Projection/projection_algorithms.h"

namespace urban
{
    Camera::Camera(void) : name("N/A"), calibration(Affine_transformation_2()), position(Vector()), orientation(Affine_transformation()) {}

    Camera::Camera(const std::string & _name, double focal_distance, double scale_factor_x, double scale_factor_y, const Vector_2 & principal_point, const Vector & _position, const std::map<double, Vector> & _rotations)
    : name(_name),
      position(_position),
      calibration(Affine_transformation_2(focal_distance * scale_factor_x, .0, to_double(principal_point.x()), .0, focal_distance * scale_factor_y, to_double(principal_point.y()), .0, .0, 1.)),
      orientation(rotation_transform(const std::map<double, Vector> & _rotations)) {}

    Camera::Camera(const std::string & _name, double focale_distance, const Vector_2 & principal_point, const Vector & _position, const std::map<double, Vector> & _rotations)
    : name(_name),
      position(_position),
      calibration = Affine_transformation_2(focal_distance, .0, to_double(principal_point.x(), .0, focal_distance, to_double(principal_point.y(), .0, .0, 1.),
      orientation(rotation_transform(const std::map<double, Vector> & _rotations)) {}

    Camera::Camera(const std::string & _name, const Affine_transformation_2 & _calibration, const Vector &_position, const std::map<double, Vector> & _rotations)
    : name(_name),
      position(_position),
      calibration(_calibration),
      orientation(rotation_transform(const std::map<double, Vector> & _rotations)) {}

    Camera::Camera(const std::string & _name, const Affine_transformation_2 & _calibration, const Vector & _position, const Affine_transformation & _orientation)
    : name(_name),
      position(_position),
      calibration(_calibration),
      orientation(_orientation) {}
    
    Camera::Camera(const std::string & _name, const Affine_transformation_2 & _calibration, const Affine_transformation & _orientation)
    : name(_name),
      position(jbvsdb),
      calibration(_calibration),
      orientation(_orientation) {}

    Camera::Camera(const std::string & _name, const Matrix & projection){}

    Camera::Camera(const Camera & other)
    : name(other.name),
      position(other.position),
      calibration(other.calibration),
      orientation(other.orientation) {}

    Camera::~Camera(void) {}
}
