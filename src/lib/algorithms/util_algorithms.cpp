#include <algorithms/util_algorithms.h>

#include <CGAL/centroid.h>

#include <iterator>

namespace city
{
    Affine_transformation_3 rotation_transform(const std::map<double, Vector_3> & _rotations)
    {
        std::vector<Affine_transformation_3> rotations;
        double norm(0);
        Vector_3 u(CGAL::NULL_VECTOR);
        std::transform(
            std::begin(_rotations),
            std::end(_rotations),
            std::begin(rotations),
            [norm, &u](const std::pair<double, Vector_3> & angle_axis) mutable
            {
                norm = std::sqrt(to_double(angle_axis.second * angle_axis.second));
                u = angle_axis.second / norm;
                return Affine_transformation_3(
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.x()), 2.), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.y()) - std::sin(angle_axis.first) * to_double(u.z()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.z()) + std::sin(angle_axis.first) * to_double(u.y()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.y()) + std::sin(angle_axis.first) * to_double(u.z()), 
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.y()), 2.), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.y()) * to_double(u.z()) - std::sin(angle_axis.first) * to_double(u.x()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.x()) * to_double(u.z()) - std::sin(angle_axis.first) * to_double(u.y()), 
                        (1 - std::cos(angle_axis.first)) * to_double(u.y()) * to_double(u.z()) + std::sin(angle_axis.first) * to_double(u.x()), 
                        std::cos(angle_axis.first) + (1 - std::cos(angle_axis.first)) * std::pow(to_double(u.z()), 2.)
                    );
            }
        );

        return std::accumulate(
            std::begin(rotations),
            std::end(rotations),
            Affine_transformation_3(CGAL::IDENTITY),
            [](Affine_transformation_3 & result, const Affine_transformation_3 & rotation)
            {
                return result * rotation;
            }
        );
    }

    bool check_collinearity(std::vector<Point_2>::iterator first, std::vector<Point_2>::iterator last)
    {
        if(std::distance(first, last) < 3)
            throw std::out_of_range("There are less than 3 points between the two iterators!");
        /**
         * It suffices to check the first three points;
         */
        return CGAL::collinear(*first, *(first + 1), *(first + 2));
    }

    std::pair<Point_2, Point_2> extrem_points(std::vector<Point_2> const& points)
    {
        Point_2 A(points.at(0)), B(points.at(1));
        double AB(.0), AC(.0), BC(.0);
        std::for_each(
            std::next(std::begin(points), 2),
            std::end(points),
            [&A, &B, &AB, &AC, &BC](Point_2 const& C)
            {
                AB = to_double(CGAL::squared_distance(A, B));
                AC = to_double(CGAL::squared_distance(A, C));
                BC = to_double(CGAL::squared_distance(B, C));
                if(AC/AB > 1 || BC/AB > 1)
                {
                    if(AB/AC > 1 || BC/AC > 1)
                        A = C;
                    else
                        B = C;
                }
            }
        );
        return std::make_pair(A, B);
    }


    InexactPoint_2 centroid(InexactPolygon const& polygon)
    {
        return CGAL::centroid(polygon.vertices_begin(), polygon.vertices_end(), CGAL::Dimension_tag<0>());
    }
    InexactPoint_2 centroid(InexactPolygon_with_holes const& polygon)
    {
        return centroid(polygon.outer_boundary());
    }
    InexactPoint_2 centroid(Polygon const& polygon)
    {
        ExactToInexact to_inexact;
        std::vector<InexactPoint_2> buffer(polygon.size());
        std::transform(
            polygon.vertices_begin(),
            polygon.vertices_end(),
            std::begin(buffer),
            [&to_inexact](Point_2 const& point)
            {
                return to_inexact(point);
            }
        );
        buffer.erase(
            std::unique(
                std::begin(buffer),
                std::end(buffer)
            ),
            std::end(buffer)
        );
        return centroid(InexactPolygon(std::begin(buffer), std::end(buffer)));
    }
    InexactPoint_2 centroid(Polygon_with_holes const& polygon)
    {
        return centroid(polygon.outer_boundary());
    }

    std::vector<double> edge_lengths(Polygon const& polygon)
    {
        std::vector<double> lengths(polygon.size(), 0);
        std::transform(
            polygon.edges_begin(),
            polygon.edges_end(),
            std::begin(lengths),
            [](Segment_2 const& edge)
            {
                return std::sqrt(
                    to_double(
                        edge.squared_length()
                    )
                );
            }
        );

        return lengths;
    }
    double circumference(Polygon const& polygon)
    {
        return std::accumulate(
            polygon.edges_begin(),
            polygon.edges_end(),
            0.,
            [](double total_length, Segment_2 const& edge)
            {
                return  total_length
                        +
                        std::sqrt(
                            to_double(
                                edge.squared_length()
                            )
                        );
            }
        );
    }
}
