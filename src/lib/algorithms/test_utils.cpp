#include <algorithms/test_utils.h>

city::projection::FacePrint test_facet_projection(std::size_t const id, city::Point_3 const& A, city::Point_3 const& B, city::Point_3 const& C)
{
    std::vector<city::Point_2> buffer{{
        city::Point_2(A.x(), A.y()),
        city::Point_2(B.x(), B.y()),
        city::Point_2(C.x(), C.y())
    }};

    return test_facet_projection(
            id,
            buffer,
            city::Plane_3(A, B, C)
        );
}



city::projection::FacePrint test_facet_projection(std::size_t const id, std::vector<city::Point_2> const& polygon_vertices, city::Plane_3 const& plane)
{
    return city::projection::FacePrint(
            id,
            city::Polygon_with_holes(
                city::Polygon(
                    std::begin(polygon_vertices),
                    std::end(polygon_vertices)
                )
            ),
            plane
        );
}



city::projection::FacePrint test_facet_projection(std::size_t const id, std::vector<city::Point_2> const& polygon_vertices, std::vector<city::Point_2> const& hole_vertices, city::Plane_3 const& plane)
{
    std::list<city::Polygon> hole_list{{city::Polygon(std::begin(hole_vertices), std::end(hole_vertices))}};

    return city::projection::FacePrint(
            id,
            city::Polygon_with_holes(
                city::Polygon(
                    std::begin(polygon_vertices),
                    std::end(polygon_vertices)
                ),
                std::begin(hole_list),
                std::end(hole_list)
            ),
            plane
        );
}
