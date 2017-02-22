#include "../libs/Algorithms/algorithms.h"
#include <CGAL/Boolean_set_operations_2.h>

#include <catch.hpp>

#include <vector>
#include <iterator>
#include <list>

SCENARIO("Occlusion management")
{
    GIVEN("two triangular faces")
    {
        urban::InexactToExact to_exact;
        urban::Point_3 A(to_exact(urban::InexactKernel::Point_3(-1., 0, 5.))),
                       B(to_exact(urban::InexactKernel::Point_3(1., 0., 2.))),
                       C(to_exact(urban::InexactKernel::Point_3(0, 1., 3.615)));
        urban::Point_3 _A(to_exact(urban::InexactKernel::Point_3(-.5, .33, 5.))),
                       _B(to_exact(urban::InexactKernel::Point_3(.5, .33, 5.))),
                       _C(to_exact(urban::InexactKernel::Point_3(0, .67, 8.2)));
        std::vector<urban::Point_2> vertices;
        vertices.push_back(urban::Point_2(A.x(), A.y()));
        vertices.push_back(urban::Point_2(B.x(), B.y()));
        vertices.push_back(urban::Point_2(C.x(), C.y()));
        
        std::vector<urban::Point_2> _vertices;
        _vertices.push_back(urban::Point_2(_A.x(), _A.y()));
        _vertices.push_back(urban::Point_2(_B.x(), _B.y()));
        _vertices.push_back(urban::Point_2(_C.x(), _C.y()));

        urban::projection::FacePrint face_1(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(vertices),
                    std::end(vertices)
                )
            ),
            urban::Plane_3(A, B, C)
        );
        urban::projection::FacePrint face_2(
            urban::Polygon_with_holes(
                urban::Polygon(
                    std::begin(_vertices),
                    std::end(_vertices)
                )
            ),
            urban::Plane_3(_A, _B, _C)
        );

        WHEN("Occlusion is computed")
        {
            std::list<urban::projection::FacePrint> rhs{{face_2}};
            std::list<urban::projection::FacePrint> result(urban::occlusion(face_1, rhs));
            THEN("the output checks:")
            {
                std::cout << std::boolalpha << CGAL::do_overlap(face_1.bbox(), face_2.bbox()) << std::endl;
                std::cout << std::boolalpha << CGAL::do_intersect(face_1.get_polygon(), face_2.get_polygon()) << std::endl;
                std::list<urban::Polygon_with_holes> superposition(0);
                CGAL::intersection(face_1.get_polygon(), face_2.get_polygon(), std::back_inserter(superposition));
                std::cout << superposition.size() << std::endl;
                        urban::Polygon_set first_parts_occluded, second_parts_occluded;
                        std::for_each(
                            std::begin(superposition),
                            std::end(superposition),
                            [&face_1, &face_2, &first_parts_occluded, &second_parts_occluded](urban::Polygon_with_holes intersection)
                            {
                                urban::Point_2 intersection_point(
                                    CGAL::centroid(
                                        intersection.outer_boundary()[0],
                                        intersection.outer_boundary()[1],
                                        intersection.outer_boundary()[2]
                                        )
                                );
                                std::cout <<  intersection_point << std::endl;
                                std::cout << face_1.get_plane_height(intersection_point) << std::endl;
                                std::cout << face_2.get_plane_height(intersection_point) << std::endl;
                                if(face_1.get_plane_height(intersection_point) > face_2.get_plane_height(intersection_point))
                                    second_parts_occluded.join(intersection);
                                else
                                    first_parts_occluded.join(intersection);
                            }
                        );
                        std::cout << second_parts_occluded.number_of_polygons_with_holes() << std::endl;
                        std::cout << first_parts_occluded.number_of_polygons_with_holes() << std::endl;
            }
        }
    }
}
