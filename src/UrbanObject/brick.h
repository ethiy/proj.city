#pragma once

#include "../ShadowMesh/shadow_mesh.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW

#include <string>
#include <ostream>

namespace urban
{
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;
    typedef CGAL::Polyhedron_3<Kernel> Polyhedron;


    /*! Container class modelling urba objects using CGAL*/
    class Brick
    {
    public:
        /*! Brick default constructor*/
        Brick(void);
        /*! Brick copy constructor*/
        Brick(const Brick &);
        /*! Brick constructor form 3ds meshes*/
        Brick(ShadowMesh);
        /*! Brick default destructor*/
        ~Brick(void);

        std::string get_name(void) const noexcept;
    private:
        std::string name;
        /*! The surface describing 3d urban objects*/
        Polyhedron surface;

        /*! Outstreaming Brick*/
        friend std::ostream& operator<<(std::ostream &, const Brick &);
        #ifdef CGAL_USE_GEOMVIEW
        friend CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream &, const Brick &);
        #endif // CGAL_USE_GEOMVIEW
    };
}
