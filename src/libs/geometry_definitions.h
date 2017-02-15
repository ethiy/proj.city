#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <CGAL/IO/Color.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Aff_transformation_3.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

#include <CGAL/intersections.h>

namespace urban
{
    template <class Refs, class Tag, class Pln>
    struct HalfedgeDS_face_colored : public CGAL::HalfedgeDS_face_base<Refs, Tag, Pln> 
    {
        void set_color(CGAL::Color _color) {color = _color;}
        CGAL::Color get_color(void) {return color;}

        CGAL::Color color;
    };

    struct Polyhedron_items_colored : public CGAL::Polyhedron_items_3
    {
        template <class Refs, class Traits>
        struct Face_wrapper
        {
            typedef HalfedgeDS_face_colored<Refs, CGAL::Tag_true, typename Traits::Plane_3> Face;
        };
    };

    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;
    typedef Kernel::Plane_3 Plane;
    typedef CGAL::Polyhedron_3<Kernel, Polyhedron_items_colored> Polyhedron;
    typedef Polyhedron::Facet Facet;
    typedef CGAL::Color Color;
    typedef CGAL::Aff_transformation_3<Kernel> Affine_transformation;
    typedef CGAL::Bbox_3 Bbox;

    typedef CGAL::Polygon_2<Kernel> Polygon;
    typedef CGAL::Polygon_with_holes_2<Kernel> Polygon_with_holes;
    typedef Kernel::Point_2 Point_2;
    typedef Kernel::Vector_2 Vector_2;
    typedef Kernel::Intersect_2 Intersect_2;
    typedef CGAL::Aff_transformation_2<Kernel> Affine_transformation_2;
    typedef CGAL::Bbox_2 Bbox_2;
}
