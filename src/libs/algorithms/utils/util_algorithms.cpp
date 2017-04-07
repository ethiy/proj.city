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
            [&A, &B, &AB, &AC, &BC](Point_2 C)
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

    InexactPoint_2 centroid(const InexactPolygon & polygon)
    {
        InexactPolygon::Vertex_const_circulator circulator = polygon.vertices_circulator();
        InexactPolygon::Vertex_const_circulator next_circulator = std::next(polygon.vertices_circulator(), 1);

        InexactVector_2 centroid = CGAL::NULL_VECTOR;
        do
        {
            centroid =  centroid
                        +
                        ((*circulator - CGAL::ORIGIN) + (*next_circulator - CGAL::ORIGIN))
                         *
                        CGAL::determinant(*circulator - CGAL::ORIGIN, *next_circulator - CGAL::ORIGIN)
                         /
                        6.;
        }while(++circulator != polygon.vertices_circulator());

        return CGAL::ORIGIN + centroid / polygon.area();
    }

    InexactPoint_2 centroid(const InexactPolygon_with_holes & polygon)
    {
        return centroid(polygon.outer_boundary());
    }

    InexactPoint_2 centroid(const Polygon & polygon)
    {
        ExactToInexact to_inexact;
        Polygon::Vertex_const_circulator circulator = polygon.vertices_circulator();
        Polygon::Vertex_const_circulator next_circulator = std::next(polygon.vertices_circulator(), 1);

        InexactVector_2 centroid = CGAL::NULL_VECTOR;
        double area = to_inexact(polygon.area());

        if(to_inexact(polygon.area()) < std::numeric_limits<double>::epsilon())
        {
            centroid = ((to_inexact(*circulator) - CGAL::ORIGIN) + (to_inexact(*next_circulator) - CGAL::ORIGIN)) / 2.;
        }
        else
        {
            InexactPoint_2 v_0, v_1;
            do
            {
                v_0 = to_inexact(*circulator);
                v_1 = to_inexact(*next_circulator);
                std::cout << v_0 << " " << v_1 << std::endl;
                centroid =  centroid
                            +
                            ((v_0 - CGAL::ORIGIN) + (v_1 - CGAL::ORIGIN))
                            *
                            CGAL::determinant(v_0 - CGAL::ORIGIN, v_1 - CGAL::ORIGIN)
                            /
                            6.
                            /
                            area;
                std::cout << centroid << std::endl;
                ++next_circulator;
                ++circulator;
            }while(circulator != std::prev(polygon.vertices_circulator(), 1));
        }
        return CGAL::ORIGIN + centroid;
    }

    InexactPoint_2 centroid(const Polygon_with_holes & polygon)
    {
        return centroid(polygon.outer_boundary());
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
