#include <algorithms/test_utils.h>

urban::projection::FacePrint test_facet_projection(urban::Point_3 const& A, urban::Point_3 const& B, urban::Point_3 const& C)
{
    std::vector<urban::Point_2> buffer{{
        urban::Point_2(A.x(), A.y()),
        urban::Point_2(B.x(), B.y()),
        urban::Point_2(C.x(), C.y())
    }};

    return urban::projection::FacePrint(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(buffer),
                    std::end(buffer)
                )
            ),
            urban::Plane_3(A, B, C)
        );
}



urban::projection::FacePrint test_facet_projection(std::vector<urban::Point_2> const& polygon_vertices, urban::Plane_3 const& plane)
{
    return urban::projection::FacePrint(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(polygon_vertices),
                    std::end(polygon_vertices)
                )
            ),
            plane
        );
}



urban::projection::FacePrint test_facet_projection(std::vector<urban::Point_2> const& polygon_vertices, std::vector<urban::Point_2> const& hole_vertices, urban::Plane_3 const& plane)
{
    std::list<urban::Polygon> hole_list{{urban::Polygon(std::begin(hole_vertices), std::end(hole_vertices))}};

    return urban::projection::FacePrint(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(polygon_vertices),
                    std::end(polygon_vertices)
                ),
                std::begin(hole_list),
                std::end(hole_list)
            ),
            plane
        );
}
