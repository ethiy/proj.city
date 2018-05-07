#pragma once

#include <projection/face_projection.h>

city::projection::FacePrint test_facet_projection(std::size_t const id, city::Point_3 const& A, city::Point_3 const& B, city::Point_3 const& C);
city::projection::FacePrint test_facet_projection(std::size_t const id, std::vector<city::Point_2> const& polygon_vertices, city::Plane_3 const& plane);
city::projection::FacePrint test_facet_projection(std::size_t const id, std::vector<city::Point_2> const& polygon_vertices, std::vector<city::Point_2> const& hole_vertices, city::Plane_3 const& plane);
