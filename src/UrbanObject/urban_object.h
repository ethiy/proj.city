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
    class UrbanObject
    {
    public:
        /*! UrbanObject default constructor*/
        UrbanObject(void);
        /*! UrbanObject copy constructor*/
        UrbanObject(const UrbanObject &);
        /*! UrbanObject constructor form 3ds meshes*/
        UrbanObject(ShadowMesh);
        /*! UrbanObject default destructor*/
        ~UrbanObject(void);

        std::string get_name(void) const noexcept;
    private:
        std::string name;
        /*! The surface describing 3d urban objects*/
        Polyhedron surface;

        /*! Outstreaming UrbanObject*/
        friend std::ostream& operator<<(std::ostream &, const UrbanObject &);
        #ifdef CGAL_USE_GEOMVIEW
        friend CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream &, const UrbanObject &);
        #endif // CGAL_USE_GEOMVIEW
    };
}