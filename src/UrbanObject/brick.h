#pragma once

#include "geometry_definitions.h"

#include "../ShadowMesh/shadow_mesh.h"

#ifdef CGAL_USE_GEOMVIEW
#include <CGAL/IO/Geomview_stream.h>
#endif // CGAL_USE_GEOMVIEW

#include <string>
#include <ostream>

namespace urban
{
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

        /*! Access brick name*/
        std::string get_name(void) const noexcept;

        /*! Face iterators*/
        typedef Polyhedron::Facet_iterator Facet_iterator;
        typedef Polyhedron::Facet_const_iterator Facet_const_iterator;
        Facet_iterator facets_begin(void) noexcept;
        Facet_iterator facets_end(void) noexcept;
        Facet_const_iterator facets_cbegin(void) const noexcept;
        Facet_const_iterator facets_cend(void) const noexcept;

        /*! Halfedge iterators*/
        typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
        typedef Polyhedron::Halfedge_const_iterator Halfedge_const_iterator;
        Halfedge_iterator halfedges_begin(void) noexcept;
        Halfedge_iterator halfedges_end(void) noexcept;
        Halfedge_const_iterator halfedges_cbegin(void) const noexcept;
        Halfedge_const_iterator halfedges_cend(void) const noexcept;

        /*! Points iterators*/
        typedef Polyhedron::Point_iterator Point_iterator;
        typedef Polyhedron::Point_const_iterator Point_const_iterator;
        Point_iterator points_begin(void) noexcept;
        Point_iterator points_end(void) noexcept;
        Point_const_iterator points_cbegin(void) const noexcept;
        Point_const_iterator points_cend(void) const noexcept;

        /*! Set Brick color*/
        void set_color(Color);
        
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
