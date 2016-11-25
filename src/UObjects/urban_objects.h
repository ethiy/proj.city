#pragma once

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <ostream>

/*! Container class modelling urba objects using CGAL*/
class UrbanObject
{
public:
    typedef CGAL::Simple_cartesian<double> Kernel;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;
    typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

    /*! UrbanObject default constructor*/
    UrbanObject();
    /*! UrbanObject copy constructor*/
    UrbanObject(const UrbanObject &);
    /*! UrbanObject constructor form 3ds meshes*/
    UrbanObject(Lib3dsMesh );
    /*! UrbanObject default destructor*/
    ~UrbanObject();
private:
    /*! The surface describing 3d urban objects*/
    Polyhedron surface;

    /*! Outstreaming UrbanObject*/
    friend std::ostream& operator<<(std::ostream &, const UrbanObject &);
};