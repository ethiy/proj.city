#pragma once

#include "Face/face.h"

#include "../geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <map>
#include <string>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Mesh
        {
        public:
            /*! Empty constructor*/
            Mesh(void);
            /*! Copy constructor*/
            Mesh(const Mesh &);
            /*! Move constructor*/
            Mesh(Mesh &&);
            /*! Convert lib3ds Mesh*/
            Mesh(Lib3dsMesh*);
            /*! Convert CGAL Polyhedron_3*/
            Mesh(const Polyhedron &);
            /*! classic constructor*/
            Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>);*
            /*! Destructor*/
            ~Mesh(void);

            /*! Swap Mesh*/
            void swap(Mesh &);

            /*! Copy assignement*/
            Mesh & operator=(const Mesh &) noexcept;
            /*! Move assignement*/
            Mesh & operator=(Mesh &&) noexcept;

            /*! Access name*/
            std::string get_name(void) const noexcept;
            /*! Access number of points*/
            size_t get_number_points(void) const noexcept;
            /*! Access point coordinates*/
            std::map<size_t, Point> get_points(void) const noexcept;
            /*! Access faces number*/
            size_t get_number_faces(void) const noexcept;
            /*! Access faces*/
            std::map<size_t, Face> get_faces(void) const noexcept;
            /*! Access bounding box*/
            Bbox bbox(void) const noexcept;

            /*! Access index of halfedge:
             *      logarithmic complexity due to the use of std::map
             */
            size_t get_index(const Polyhedron::Halfedge & h);

            /*! Convert Mesh to lib3ds mesh structure*/
            Lib3dsMesh* to_3ds(void);
        private:
            /*! Mesh name*/
            std::string name;
            /*! Coordinates*/
            std::map<size_t, Point> points;
            /*! Faces*/
            std::map<size_t, Face> faces;
            /*! Bounding box*/
            Bbox bounding_box;
            /*! Compute bounding box internal method*/
            void compute_box(void);
            /*! Write Mesh to stream*/
            friend std::ostream& operator<<(std::ostream &, const Mesh &);
        };
    }

    /*! Swap utility for Mesh*/
    void swap(shadow::Mesh &, shadow::Mesh &);
}
