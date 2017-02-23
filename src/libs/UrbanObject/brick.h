#pragma once

#include "../geometry_definitions.h"

#include "../Shadow/mesh.h"

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
        Brick(const Brick & other);
        /** Brick move constructor*/
        Brick(Brick && other);
        /*! Brick constructor form 3ds meshes*/
        Brick(shadow::Mesh mesh);
        /*! Brick default destructor*/
        ~Brick(void);

        /**
         * Swap `this` with `other`.
         * @param other an other brick to swap with
         * @see swap(Brick &, Brick &)
         */
        void swap(Brick &other);
            
        /**
         * Copy assignement operator.
         * @param other an other brick to copy
         * @see operator=(Brick &&)
         */
        Brick & operator=(const Brick & other) noexcept;

        /**
         * Move assignement operator.
         * @param other an other brick to move
         * @see operator=(const Brick &)
         */
        Brick & operator=(Brick && other) noexcept;

        /*! Access brick name*/
        std::string get_name(void) const noexcept;
        
        /*! Get the number of vertices*/
        size_t vertices_number(void) const;

        /*! Get the number of faces*/
        size_t facets_number(void) const;

        /*! Access Bounding box*/
        Bbox_3 bbox(void) const noexcept;

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
        Halfedge_iterator border_halfedges_begin(void) noexcept;
        Halfedge_const_iterator border_halfedges_begin(void) const noexcept;

        /*! Points iterators*/
        typedef Polyhedron::Point_iterator Point_iterator;
        typedef Polyhedron::Point_const_iterator Point_const_iterator;
        Point_iterator points_begin(void) noexcept;
        Point_iterator points_end(void) noexcept;
        Point_const_iterator points_cbegin(void) const noexcept;
        Point_const_iterator points_cend(void) const noexcept;

        /*! Plane iterators*/
        typedef Polyhedron::Plane_iterator Plane_iterator;
        typedef Polyhedron::Plane_const_iterator Plane_const_iterator;
        Plane_iterator planes_begin(void) noexcept;
        Plane_iterator planes_end(void) noexcept;
        Plane_const_iterator planes_cbegin(void) const noexcept;
        Plane_const_iterator planes_cend(void) const noexcept;

        /*! Set Brick color*/
        void set_color(Color);

    private:
        /* ! Brick name*/
        std::string name;
        /*! The surface describing 3d urban objects*/
        Polyhedron surface;
                /*! Bounding box*/
        Bbox_3 bounding_box;

        /*! Outstreaming Brick*/
        friend std::ostream& operator<<(std::ostream &, const Brick &);
        #ifdef CGAL_USE_GEOMVIEW
        friend CGAL::Geomview_stream& operator<<(CGAL::Geomview_stream &, const Brick &);
        #endif // CGAL_USE_GEOMVIEW
    };

    /**
     * Swaps two bricks.
     * @param lhs left-hand Brick.
     * @param rhs right-hand Brick.
     */
    void swap(Brick & lhs, Brick &rhs);
}
