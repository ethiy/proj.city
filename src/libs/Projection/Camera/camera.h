#pragma once

#include "../../geometry_definitions.h"

#include <map>

namespace urban
{
    namespace projection
    {
        class Camera
        {
        public:
            Camera(void);
            Camera(const std::string & _name, double focal_distance, double scale_factor_x, double scale_factor_y, const Vector_2 & principal_point, const Vector & _position, const std::map<double, Vector> & _rotations);
            Camera(const std::string & _name, double focal_distance, const Vector_2 & principal_point, const Vector & _position, const std::map<double, Vector> & _rotations);
            Camera(const std::string & _name, const Affine_transformation_2 & _calibration, const Vector & _position, const std::map<double, Vector> & _rotations);
            Camera(const std::string & _name, const Affine_transformation_2 & _calibration, const Vector & _position, const Affine_transformation & _orientation);
            Camera(const Camera & other);
            Camera(Camera && other);
            ~Camera(void);

            void swap(Camera & other);

            Camera & operator=(const Camera & other) noexcept;

            Camera & operator=(Camera && other) noexcept;
        private:
            std::string name;
            Affine_transformation_2 calibration;
            Vector position;
            Affine_transformation orientation;
        };
    }

    void swap(projection::Camera & lhs, projection::Camera & rhs);
}
