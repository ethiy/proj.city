#pragma once

#include <CGAL/Homogeneous.h>

#include <CGAL/IO/Color.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Aff_transformation_3.h>

#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Polygon_2.h>

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

    typedef CGAL::Homogeneous<double> Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;
    typedef CGAL::Polyhedron_3<Kernel, Polyhedron_items_colored> Polyhedron;
    typedef Polyhedron::Facet Facet;
    typedef CGAL::Color Color;
    typedef CGAL::Aff_transformation_3<Kernel> Affine_transformation;
    typedef CGAL::Bbox_3 Bbox;

    typedef CGAL::Projection_traits_xy_3<Kernel> Projective_traits;
    typedef CGAL::Polygon_2<Projective_traits> Polygon;
    typedef Projective_traits::Point_2 Point_2;
    typedef Projective_traits::Vector_2 Vector_2;
}
