#pragma once

#include <projection/face_projection.h>

urban::projection::FacePrint test_facet_projection(std::size_t const id, urban::Point_3 const& A, urban::Point_3 const& B, urban::Point_3 const& C);
urban::projection::FacePrint test_facet_projection(std::size_t const id, std::vector<urban::Point_2> const& polygon_vertices, urban::Plane_3 const& plane);
urban::projection::FacePrint test_facet_projection(std::size_t const id, std::vector<urban::Point_2> const& polygon_vertices, std::vector<urban::Point_2> const& hole_vertices, urban::Plane_3 const& plane);
