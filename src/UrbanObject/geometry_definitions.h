#pragma once

#include <CGAL/Homogeneous.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Polyhedron_3.h>

#include <CGAL/Aff_transformation_3.h>


namespace urban
{
    template <class T>
    struct HalfedgeDS_face_colored : CGAL::HalfedgeDS_face_base<T> 
    {
        void set_color(CGAL::Color _color) {color = _color;}
        CGAL::Color get_color(void) {return color;}

        CGAL::Color color;
    };

    struct Polyhedron_items_colored : CGAL::Polyhedron_items_3 {
        template <class T, class Traits>
        struct Face_wrapper
        {
            typedef HalfedgeDS_face_colored<T> Face;
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
}
