#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Cartesian_converter.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Aff_transformation_3.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <CGAL/intersections.h>

namespace urban
{
    typedef CGAL::Simple_cartesian<double> InexactKernel;
    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
    typedef CGAL::Cartesian_converter<Kernel, InexactKernel> ExactToInexact;
    typedef CGAL::Cartesian_converter<InexactKernel, Kernel> InexactToExact;

    typedef Kernel::Point_3 Point_3;
    typedef Kernel::Vector_3 Vector_3;
    typedef Kernel::Plane_3 Plane_3;
    typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
    typedef Polyhedron::Facet Facet;
    typedef CGAL::Color Color;
    typedef CGAL::Aff_transformation_3<Kernel> Affine_transformation_3;
    typedef CGAL::Bbox_3 Bbox_3;

    typedef CGAL::Polygon_2<Kernel> Polygon;
    typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes;
    typedef CGAL::Polygon_set_2<Kernel> Polygon_set;
    typedef Kernel::Point_2 Point_2;
    typedef InexactKernel::Point_2 InexactPoint_2;
    typedef Kernel::Vector_2 Vector_2;
    typedef Kernel::Intersect_2 Intersect_2;
    typedef CGAL::Aff_transformation_2<Kernel> Affine_transformation_2;
    typedef CGAL::Bbox_2 Bbox_2;
}
