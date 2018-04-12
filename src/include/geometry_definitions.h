#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Cartesian_converter.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

#include <CGAL/Nef_polyhedron_3.h>


#include <CGAL/Aff_transformation_3.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

#include <CGAL/intersections.h>

namespace urban
{
    using InexactKernel = CGAL::Simple_cartesian<double>;
    using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
    using ExactToInexact = CGAL::Cartesian_converter<Kernel, InexactKernel>;
    using InexactToExact = CGAL::Cartesian_converter<InexactKernel, Kernel>;

    using Point_3 = Kernel::Point_3;
    using Vector_3 = Kernel::Vector_3;
    using Plane_3 = Kernel::Plane_3;
    using Polyhedron = CGAL::Polyhedron_3<Kernel, CGAL::Polyhedron_items_with_id_3>;
    using Nef_Polyhedron =  CGAL::Nef_polyhedron_3<Kernel>;
    using Color = CGAL::Color;
    using Affine_transformation_3 = CGAL::Aff_transformation_3<Kernel>;
    using Bbox_3 = CGAL::Bbox_3;

    using Polygon = CGAL::Polygon_2<Kernel>;
    using InexactPolygon = CGAL::Polygon_2<InexactKernel>;
    using Polygon_with_holes = CGAL::Polygon_with_holes_2<Kernel>;
    using InexactPolygon_with_holes = CGAL::Polygon_with_holes_2<InexactKernel>;
    using Polygon_set = CGAL::Polygon_set_2<Kernel>;
    using Point_2 = Kernel::Point_2;
    using InexactPoint_2 = InexactKernel::Point_2;
    using Vector_2 = Kernel::Vector_2;
    using Segment_2 = Kernel::Segment_2;
    using InexactVector_2 = InexactKernel::Vector_2;
    using Intersect_2 = Kernel::Intersect_2;
    using Affine_transformation_2 = CGAL::Aff_transformation_2<Kernel>;
    using Bbox_2 = CGAL::Bbox_2;
}
