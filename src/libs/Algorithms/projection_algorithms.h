#pragma once

#include "../geometry_definitions.h"
#include "../UrbanObject/brick.h"
#include "../Projection/projection.h"

#include <vector>
#include <list>

namespace urban
{
    /** Projects on camera view*/
    projection::BrickPrint project(const Brick & brick, const projection::Camera & camera);

    /* ! Projects on xy axis*/
    projection::BrickPrint project(const Brick & brick);

    /**
     * Rasterize projection
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @param image_sizes the image size
     * @return The image matrix in form of an array
     */
    std::vector<double> rasterize(const projection::BrickPrint & brick_projection, double pixel_size, std::pair<size_t, size_t> & image_sizes);

    /**
     * Rasterize projection
     * @tparam dynamic_type type of the dynamic
     * @param brick_projection the projection to rasterize
     * @param pixel_size the pixel size
     * @param image_sizes the image size
     * @return The image matrix in form of an array with desired dynamic
     */
    template<typename dynamic_type> std::vector<dynamic_type> rasterize_to(const projection::BrickPrint & brick_projection, double pixel_size, std::pair<size_t, size_t> & image_sizes);
    
    /** Project Faces to XY*/
    std::vector<projection::FacePrint> project_xy(const Brick & brick);

    /** Computes FacePrint occlutions and gets rid of all perpendicular facets*/
    std::list<projection::FacePrint> occlusion(const projection::FacePrint & lhs, std::list<projection::FacePrint> & rhs);
}
