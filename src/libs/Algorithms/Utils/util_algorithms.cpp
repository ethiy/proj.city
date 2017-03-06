#include "util_algorithms.h"

#include <iterator>

namespace urban
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
            [norm, u](const std::pair<double, Vector_3> & angle_axis) mutable
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

    void extrem_points(std::vector<Point_2> & points)
    {
        Point_2 A(points[0]), B(points[1]);
        double AB(.0), AC(.0), BC(.0);
        std::for_each(
            std::next(std::begin(points), 2),
            std::end(points),
            [&](Point_2 C)
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
        points.clear();
        points.push_back(A);
        points.push_back(B);
    }

    template<typename dynamic_type> void convert(const std::vector<double> & original_image, std::vector<dynamic_type> & target_image)
    {
        unsigned long long dynamic = std::pow(2, sizeof(dynamic_type) * 8) - 1;
        target_image = std::move(std::vector<dynamic_type>(original_image.size()));
        std::vector<double>::iterator maximum_it = std::max_element(std::begin(original_image), std::end(original_image));
        std::transform(
            std::begin(original_image),
            std::end(original_image),
            std::begin(target_image),
            [dynamic, &maximum_it](const double value)
            {
                return static_cast<dynamic_type>( dynamic * value / (*maximum_it));
            }
        );
    }


    Heuristic::Heuristic(void){}
    Heuristic::Heuristic(const Heuristic &){}
    Heuristic::Heuristic(Heuristic &&){}
    Heuristic::~Heuristic(void){}

    SimpleHeuristic::SimpleHeuristic(void){}
    SimpleHeuristic::SimpleHeuristic(const SimpleHeuristic &){}
    SimpleHeuristic::SimpleHeuristic(SimpleHeuristic &&){}
    SimpleHeuristic::~SimpleHeuristic(void){}
    bool SimpleHeuristic::operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b)
    {
        /* If one of the faces is perpendicular do not bother changing order
         */
        bool greater(false); 
        if(!facet_a.is_perpendicular() && !facet_b.is_perpendicular())
        {
            Point_2 point_a(
                CGAL::centroid(
                    facet_a.outer_boundary()[0],
                    facet_a.outer_boundary()[1],
                    facet_a.outer_boundary()[2]
                )
            );
            Point_2 point_b(
                CGAL::centroid(
                    facet_b.outer_boundary()[0],
                    facet_b.outer_boundary()[1],
                    facet_b.outer_boundary()[2]
                )
            );
            greater = facet_b.get_plane_height(point_b) < facet_a.get_plane_height(point_a);
        }
        return greater;
    }

    NaiveHeuristic::NaiveHeuristic(void){}
    NaiveHeuristic::NaiveHeuristic(const NaiveHeuristic &){}
    NaiveHeuristic::NaiveHeuristic(NaiveHeuristic &&){}
    NaiveHeuristic::~NaiveHeuristic(void){}
    bool NaiveHeuristic::operator()(const projection::FacePrint & facet_a, const projection::FacePrint & facet_b)
    {
        bool greater(false); 
        if(!facet_a.is_perpendicular() && !facet_b.is_perpendicular())
        {
            auto m_a = std::min_element(
                facet_a.outer_boundary().vertices_begin(),
                facet_a.outer_boundary().vertices_end(),
                [& facet_a](const Point_2 & A, const Point_2 & B)
                {
                    return facet_a.get_plane_height(A) < facet_a.get_plane_height(B);
                }
            );
            auto m_b = std::max_element(
                facet_b.outer_boundary().vertices_begin(),
                facet_b.outer_boundary().vertices_end(),
                [& facet_b](const Point_2 & A, const Point_2 & B)
                {
                    return facet_b.get_plane_height(A) < facet_b.get_plane_height(B);
                }
            );
            greater = facet_b.get_plane_height(*m_b) < facet_a.get_plane_height(*m_a);
        }
        return greater;
    }
}
