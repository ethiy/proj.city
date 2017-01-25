#pragma once

#include "../geometry_definitions.h"

#include <vector>

namespace urban
{
    class Camera
    {
    public:
        Camera(void);
        Camera(const std::string &, double, double, double, const Vector_2 &, const Vector &, const std::map<double, Vector> &);
        Camera(const std::string &, double, const Vector_2 &, const Vector &, const std::map<double, Vector> &);
        Camera(const std::string &, const Affine_transformation_2 &, const Vector &, const std::map<double, Vector> &);
        Camera(const std::string &, const Affine_transformation_2 &, const Vector &, const Affine_transformation &);
        Camera(const std::string &, const Affine_transformation_2 &, const Affine_transformation &);
        Camera(const std::string &, const CGAL::Matrix);
        Camera(const Camera &);
        ~Camera(void);
    private:
        std::string name;
        Affine_transformation_2 calibration;
        Vector position;
        Affine_transformation orientation;
    };
}
